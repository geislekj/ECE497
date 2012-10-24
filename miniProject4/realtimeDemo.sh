node ./realtimeDemo.js &
NODE_PID=$!
sleep 1

midori --app="localhost:8080/buttonBox.html"
kill -15 $NODE_PID

