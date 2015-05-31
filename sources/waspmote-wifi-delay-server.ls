require! net
require! fs
port = 9988
file = fs.createWriteStream "#__dirname/waspmote-data.txt"
s = net.createServer (connection) ->
    f = fs.createWriteStream "wifi-latency.tsv"
    count = 0
    amount = 0
    start = Date.now!
    # console.log "Connection!"
    firstPacketTime = null
    lastPacketTime = null
    connection.on \data ->

        count++
        amount += it.length
        firstPacketTime := Date.now! if firstPacketTime is null
        lastPacketTime := Date.now!
        console.log "#{it}\t#{Date.now!}"
        # process.stdout.write "."
        # console.log it.toString!
        if it.length < 80
            val = parseInt it.toString!, 10
            val /= 100
            # f.write "#{val}\n"

    connection.on \end ->
        # console.log "Bye!"
        # t = lastPacketTime - firstPacketTime
        # console.log "Received #count packages in #{t/1000}s"
        # console.log "#{count}\t#{amount}\t#{t}"
        # f.close!
<~ s.listen port
console.log "Listening on #port"

