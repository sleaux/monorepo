module.exports = function (req, res) {
    res.send(`
<!doctype html>
<html>
  <head>
    <title>WASM Demo</title>
  </head>
  <body>
    <h3>WASM Demo</h3>
  </body>
  <script src="loader.js"></script>
</html>`);
}
 

