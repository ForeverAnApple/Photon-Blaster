// This app will be deployed to Heroku
let express = require('express')
var bodyParser = require('body-parser')
let app = express()
let url = require('url')

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded

// GET method route
app.get('/', function (request, response) {
    console.log("Query" + JSON.stringify(request.query))
    console.log("Pulse: " + request.query.pulse)
    console.log("Oxygen: " + request.query.oxygen)
    response.send('GET request to the homepage\n')
})

// POST method route
app.post('/', function (request, response) {
  console.log("Body: " + JSON.stringify(request.body))
  console.log("Pulse: " + request.body.pulse)
  console.log("Oxygen: " + request.body.oxygen)
  response.send('POST request to the homepage\n')
})

app.listen(process.env.PORT || 3000, () => console.log('Example app listening on port 3000!'))