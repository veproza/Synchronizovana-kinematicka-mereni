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
  [17000 19000]
  [14200 16200]

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
    meta = data.readUInt16BE i
    value = data.readInt16BE i + 2
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
      for datum in currentBatch
        datum.timeRatio = ratio
        # unless zeroBands[index].0 < datum.value < zeroBands[index].1
        if 1.2879e9 < (seconds * 1e6 + datum.time) < 1.2891e9
          out += "#{datum.sync}\t#{toHumanTime seconds * 1e6 + datum.time * ratio}\t#{seconds * 1e6 + datum.time}\t#{datum.value}\t#{datum.timeRatio}\t#{datum.origTime}\n"
      ++seconds
      currentBatch.length = 0

    lastMinitime = minitime
    datum = {sync, time, value, origTime}
    currentBatch.push datum
    lastSync = sync


  fs.writeFileSync "#__dirname/#{filename}.tsv" out#.substr 0, 2000000
