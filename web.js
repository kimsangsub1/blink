const express = require('express')
const app = express()
const fs = require('fs')

app.get('/update', function(req,res){
	fs.appendFile('log.txt',JSON.stringify(req.query)+"<p>",function(err){
		if(err) throw err
		res.end("Got " + String(seq++) + " " + JSON.stringify(req.query))
	})
})
app.get('/get', function(req, res) {
    value = req.query.count;
    fs.readFile('log.txt', 'UTF-8', function(err,data) {
    res.send(data);
    });
})
/*
var seq = 0
app.get('/log', function(req, res){
	fs.appendFile('log.txt',JSON.stringify(req.query)+"\n", function (err){
		if(err) throw err
		console.log("%j is updated",req.query)
		res.end("Got " + String(seq++) + " " + JSON.stringify(req.query))
	});
})
*/
app.listen(3000, () => console.log('Example app listening on port 3000!'))
