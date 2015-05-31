require! net
require! fs
port = 9988

s = net.createServer (connection) ->
    f = fs.createWriteStream "wifi-latency.tsv"
    i = 0
    amount = 0
    start = Date.now!
    console.log "Connection!"
    connection.on \data ->
        i++
        amount += it.length
        # process.stdout.write "."
        console.log it.toString!
        if it.length < 80
            val = parseInt it.toString!, 10
            val /= 100
            # f.write "#{val}\n"

    connection.on \end ->
        process.stdout.write "\n"
        console.log "Bye!"
        t = Date.now! - start
        console.log "Received #i packages in #{t/1000}s"
        console.log "Speed: #{amount / t}kBps"
        f.close!
<~ s.listen port
console.log "Listening on #port"

