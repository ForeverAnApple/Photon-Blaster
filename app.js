const express = require('express')  // require express
const app = express()  // create a request handler function
const path = require('path')

const port = process.env.PORT || 8081

// include the static client-side files (.html, .css, .js)
app.use(express.static(path.join(__dirname)))

// on a GET request to default page, serve up our index.html....
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname, 'src', 'index.html'))
})

// this will execute for all unknown URIs not specifically handled
app.get(function (req, res) {
 res.render("404")
})

// Listen for an app request on port - use backtics with variable
app.listen(port, function () {
  console.log(`Node js server running!`)
})