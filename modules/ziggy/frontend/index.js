const ibazelLivereloadUrl = process.env.IBAZEL_LIVERELOAD_URL
const httpServer = require('http').createServer().listen(3000)
var httpHandler;

async function reloadServer() {
    // clean-up require's cache on live reload so that the server is forced to reload
    // the newly built code from the file-system.
    Object.keys(require.cache).forEach(function(key) { delete require.cache[key] })

    // if an handler has already been registered before, then remove the previous one
    // and replce it with the newly imported one.
    if (httpHandler) {
        httpServer.removeListener('request', httpHandler)
    }

    server = require('./server.js')
    httpHandler = server.createHandler(ibazelLivereloadUrl)
    httpServer.on('request', httpHandler)
}

reloadServer()

if (ibazelLivereloadUrl) {
    const readline = require('readline')
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        terminal: false
    });
    rl.on('line', async (line) => {
        if (line === "IBAZEL_BUILD_COMPLETED SUCCESS") {
            await reloadServer()
        }
    });
}
