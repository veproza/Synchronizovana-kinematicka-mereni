require! fs
toHumanTime = (microStamp) ->
  microStamp = Math.round microStamp
  micros = (microStamp % 1000000) / 1000000
  seconds = Math.floor microStamp / 1000000
  minutes = Math.floor seconds / 60
  seconds = seconds % 60
  "#{minutes}:#{seconds + micros}"

totalMaxTime = null
zeroBands =
  [18000 19000]
  [14700 15700]
for filename, index in <[gps-data1 gps-data2]>
  data = fs.readFileSync "#__dirname/#{filename}.bin"
  measurementStarted = no
  lastSync = null
  lastMinitime = null
  time = 0
  origTime = 0
  currentBatch = []
  out = ""
  seconds = 0
  for i in [0 to data.length - 4  by 4]
    meta = data.readInt16BE i
    value = data.readUInt16BE i + 2
    sync = Math.floor meta / (2^14)
    minitime = meta % (2^14)
    # console.log value, sync, minitime
    continue if value == -1
    if measurementStarted == no and sync == 1 and lastSync == 0
      measurementStarted = yes
      lastMinitime = minitime
    if not measurementStarted
      lastSync = sync
      continue

    timeDiff = minitime - lastMinitime
    if timeDiff < 0 # 10ms overflow
      timeDiff += 10000
    time += timeDiff
    origTime += timeDiff

    if sync == 1 and lastSync == 0 and currentBatch.length > 1
      batchTimeSpan = (time) - currentBatch[0].time
      expectedSpan = 1e6 * Math.round batchTimeSpan / 1e6
      ratio = expectedSpan / batchTimeSpan
      time = 0
      for datum in [currentBatch[0]]
        datum.timeRatio = ratio
        out += "#{datum.sync}\t#{toHumanTime seconds * 1e6 + datum.time * ratio}\t#{seconds * 1e6 + datum.time}\t#{datum.origTime}\t#{datum.value}\t#{datum.timeRatio}\n"
      ++seconds
      currentBatch.length = 0

    lastMinitime = minitime
    datum = {sync, time, value, origTime}
    currentBatch.push datum
    lastSync = sync


  fs.writeFileSync "#__dirname/#{filename}.tsv" out.substr 0, 200000
