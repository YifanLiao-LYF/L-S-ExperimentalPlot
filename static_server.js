// static_server.js - tiny static server with proper Content-Type mapping
const http = require('http');
const fs = require('fs');
const path = require('path');
const root = process.cwd();
const port = process.env.PORT ? Number(process.env.PORT) : 8080;

const MIME = {
  '.html': 'text/html; charset=utf-8',
  '.js': 'text/javascript; charset=utf-8',
  '.mjs': 'text/javascript; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.json': 'application/json; charset=utf-8',
  '.png': 'image/png',
  '.jpg': 'image/jpeg',
  '.jpeg': 'image/jpeg',
  '.svg': 'image/svg+xml',
  '.ico': 'image/x-icon',
  '.woff': 'font/woff',
  '.woff2': 'font/woff2',
  '.ttf': 'font/ttf',
  '.map': 'application/json; charset=utf-8',
  '.wasm': 'application/wasm'
};

function send404(res){ res.statusCode = 404; res.setHeader('Content-Type','text/plain; charset=utf-8'); res.end('Not found'); }

const server = http.createServer((req, res) => {
  try{
    let urlPath = decodeURIComponent(req.url.split('?')[0]);
    if (urlPath === '/' || urlPath === '') urlPath = '/index.html';
    const p = path.join(root, urlPath);
    if (!p.startsWith(root)) { res.statusCode = 403; res.end('Forbidden'); return; }
    fs.stat(p, (err, st) => {
      if (err) return send404(res);
      if (st.isDirectory()) {
        const idx = path.join(p, 'index.html');
        return fs.stat(idx, (ie, isidx) => {
          if (ie) return send404(res);
          streamFile(idx, res);
        });
      }
      streamFile(p, res);
    });
  } catch(e){ console.error('Request error', e); res.statusCode=500; res.end('Server error'); }
});

function streamFile(filePath, res){
  const ext = path.extname(filePath).toLowerCase();
  const ct = MIME[ext] || 'application/octet-stream';
  res.setHeader('Content-Type', ct);
  const stream = fs.createReadStream(filePath);
  stream.on('error', () => send404(res));
  stream.pipe(res);
}

server.listen(port, () => console.log(`STATIC SERVER ${root} -> http://localhost:${port}`));

// graceful shutdown on SIGINT
process.on('SIGINT', () => { console.log('Server shutting down'); server.close(() => process.exit(0)); });
