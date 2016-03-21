var addon = require('bindings')('addon.node')
var express = require('express');
var app = express();

app.use(express.static('public'));

app.get('/index.html', function (req, res) {
   res.sendFile( __dirname + "/" + "index.html" );
})

app.get('/process_get', function (req, res) {
   response = addon.getSuggestedReading( req.query.title, Number( req.query.number ) );
   res.end("<html><meta charset=\"utf-8\"><body>"+response+"</body></html>");
})

var server = app.listen(8081, function () {

  var host = server.address().address
  var port = server.address().port

  console.log("Example app listening at http://%s:%s", host, port)

})

