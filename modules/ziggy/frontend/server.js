const express = require('express')

const { stringReplace } = require('string-replace-middleware')

module.exports.createHandler = function(ibazelLivereloadUrl) {
    const defaultRoute = require('./default_route.js')
    const helloRoute = require('./hello_route.js')
    const app = express()
    if (ibazelLivereloadUrl) {
        app.use(stringReplace({
            '<head>': `<head><script src="${ibazelLivereloadUrl}"></script>`
        }))
    }
    app.use(express.static('public'))
    app.get('/', defaultRoute)
    app.get('/who', helloRoute)
    return app
}
