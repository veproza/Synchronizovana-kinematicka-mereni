require! serialport.SerialPort
require! request
require! fs
require! net
require! async

baudrate = 115200
sp = new SerialPort "COM8", {baudrate}
file = fs.createWriteStream "wifi-duplex.tsv"
sp.on \data ->
  d = it.toString!
  for i in d.split "\r\n"
    console.log "| #i"

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

port = 9988

t_connection_start = null #
t_connection_established = null #
t_data_tx_send = null #
t_data_tx_received = null #
t_data_rx_send = null #
t_data_rx_received = null #
t_disconnection_start = null #
t_disconnection_complete = null #


write = -> # process.stdout.write it
server = net.createServer (connection) ->
  f = fs.createWriteStream "wifi-latency.tsv"
  i = 0
  amount = 0
  t_connection_established := Date.now!
  connection.on \data ->
    write "A"
    t_data_tx_received := Date.now!
    <~ setTimeout _, 200
    t_data_rx_send := Date.now!
    <~ connection.write "RX"
    write "B"


  connection.on \end ->
    write "C"
    t_disconnection_complete := Date.now!
<~ server.listen port
console.log "Listening on #port"



console.log '#open'
<~ sp.once \open
<~ sp.write "AT+RST\r\n"
<~ waitFor "ready"
console.log "Waiting for DHCP and such"
<~ setTimeout _, 5000
sendData = (data, cb) ->
  len = data.length
  write "4"
  <~ sp.write "AT+CIPSEND=#{len}\r\n"
  write "5"
  <~ waitFor "\r\n>"
  write "6"
  t_data_tx_send := Date.now!
  <~ sp.write data
  write "7"
  <~ waitFor "\r\nSEND OK"
  write "8"
  cb!
<~ async.eachSeries [0 to 1000], (i, cb) ->
  t_connection_start := null
  t_connection_established := null
  t_data_tx_send := null
  t_data_tx_received := null
  t_data_rx_send := null
  t_data_rx_received := null
  t_disconnection_start := null
  t_disconnection_complete := null

  write "1"
  t_connection_start := Date.now!
  waitFor "\r\nUnlink" ->
    times = [t_connection_start, t_connection_established, t_data_tx_send, t_data_tx_received, t_data_rx_send, t_data_rx_received, t_disconnection_start, t_disconnection_complete]
    file.write "#{times.join '\t'}\n"
    console.log times.join "\t"
    write "E"
    setTimeout _, 100
    # console.log "---------"
    cb!

  <~ sp.write 'AT+CIPSTART="TCP","192.168.6.117",9988\r\n'
  write "2"
  <~ waitFor "Linked"
  write "3"
  dataToSend = new Buffer "TX"
  waitFor "+IPD" ->
    write "C"
    t_data_rx_received := Date.now!
    setTimeout _, 100
    t_disconnection_start := Date.now!
    <~ sp.write "AT+CIPCLOSE\r\n"
    write "D"
    <~ waitFor "\r\nUnlink"
  <~ sendData dataToSend
  write "9"
console.log "-------------------------------"
console.log '\n\nDone!'
process.exit!
# <~ sp.close!
