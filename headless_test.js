// This test script was archived on 2025-10-21.
// The original headless E2E test used Puppeteer to open the local server, simulate answering a question,
// take a screenshot and exit. Per user request the script has been archived/moved to scripts/archive/.
//
// If you need to permanently delete this file, remove it from the repository. It is safe to remove as
// it only serves as a development-time E2E helper and is not required by the production app.

const puppeteer = require('puppeteer');

(async () => {
	const browser = await puppeteer.launch({ headless: true });
	const page = await browser.newPage();
	page.on('console', msg => console.log('[browser]', msg.text()));

	const base = 'http://localhost:8000/';
	console.log('Opening', base);
	await page.goto(base, { waitUntil: 'networkidle2' });

	// wait for level grid and click first unlocked "开始" button
	await page.waitForSelector('.level-card .btn.btn-primary');
	const startButtons = await page.$$('.level-card.unlocked .btn.btn-primary');
	if (!startButtons || startButtons.length === 0) {
		console.error('No start buttons found (no unlocked levels?)');
		await browser.close();
		process.exit(2);
	}

	console.log('Clicking first start button');
	await startButtons[0].click();

	// Wait for question view to be active
	await page.waitForSelector('#question.active, #question.view.active, section#question.active', { timeout: 3000 }).catch(e => {});
	// small delay for scripts
	await page.waitForTimeout(300);

	// try to read question text and expected answer from JS QuizManager if accessible
	const qText = await page.$eval('.question-text', el => el.textContent.trim());
	console.log('Question text:', qText);

	// If the question text is a simple arithmetic like "a + b" or similar, try to eval
	let computedAnswer = null;
	try {
		const js = qText.replace('×', '*').replace('÷', '/').replace('。', '').replace(' ', '');
		// naive regex to test if it's like 3+4 or (3+4)*2 etc.
		if (/^[0-9()\+\-\*\/\.\s]+$/.test(js)) {
			computedAnswer = eval(js).toString();
			console.log('Computed numeric answer:', computedAnswer);
		}
	} catch (e) {
		// ignore
	}

	// fill answer input
	const inputSel = '#answer-input';
	await page.waitForSelector(inputSel);
	if (computedAnswer !== null) {
		await page.focus(inputSel);
		await page.keyboard.type(computedAnswer);
	} else {
		// type a dummy value
		await page.focus(inputSel);
		await page.keyboard.type('0');
	}

	// click submit
	const submitBtn = await page.$('.answer-form .btn.btn-primary');
	if (!submitBtn) {
		console.error('Submit button not found');
		await browser.close();
		process.exit(3);
	}
	await submitBtn.click();

	// wait for short period to allow UI to react and possibly navigate
	await page.waitForTimeout(1500);

	// check which view is active now
	const activeView = await page.evaluate(() => {
		const active = document.querySelector('.view.active');
		return active ? active.id || active.getAttribute('id') : null;
	});

	console.log('Active view after submitting first question:', activeView);

	// take a screenshot for inspection
	await page.screenshot({ path: 'headless_result.png', fullPage: true });
	console.log('Screenshot saved to headless_result.png');

	await browser.close();
	// exit with code indicating whether it returned to levels
	if (activeView === 'levels') process.exit(10);
	else process.exit(0);
})();
