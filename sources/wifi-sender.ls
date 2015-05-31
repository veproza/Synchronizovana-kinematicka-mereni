require! serialport.SerialPort
require! request
require! fs
require! async

baudrate = 115200
sp = new SerialPort "COM8", {baudrate}

sp.on \data ->
  d = it.toString!
  # for i in d.split "\r\n"
  #   console.log "| #i"

waitFor = (str, cb) ->
  # console.log ".."
  datalog = ""
  check = ->
    data = it.toString!
    datalog += data
    if 0 <= datalog.indexOf str
      sp.removeListener \data check
      # console.log "!"
      cb!
  sp.on \data check

console.log '#open'
<~ sp.once \open
<~ sp.write "AT+RST\r\n"
<~ waitFor "ready"
console.log "Waiting for DHCP and such"
<~ setTimeout _, 5000
<~ sp.write 'AT+CIPSTART="TCP","192.168.6.117",9988\r\n'
<~ waitFor "Linked"
f = fs.createWriteStream "#__dirname/wifi-send-latency.tsv"
sendData = (data, cb) ->
  len = data.length
  <~ sp.write "AT+CIPSEND=#{len}\r\n"
  <~ waitFor "\r\n>"
  <~ sp.write data
  <~ waitFor "\r\nSEND OK"
  cb!
dataToSend = new Buffer "0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ--------0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz9876543210ABCDEFGHIJKLMNOPQRSTUVWXYZ--------"
<~ async.eachSeries [0 to 10000], (i, cb) ->
  t1 = Date.now!
  <~ sendData dataToSend
  t2 = Date.now!
  console.log t2 - t1
  f.write "#{t2 - t1}\n"
  cb!


console.log '#CIPCLOSE'
<~ sp.write "AT+CIPCLOSE\r\n"

console.log '#closing'
# <~ sp.close!
