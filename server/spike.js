let name = "";  // computer name string for GET request to JTAG client
let port = "";  // port number string for GET request to JTAG client
var si;  // periodic interval timer for GUI refresh
var eSource;  // event dsource for synthesizer output update

function sse_listener(event) {
   if(name == "") {
      var s = event.data.indexOf("Listening for debug commands on ");
      if( s == -1 ) { // if the "Listening..." string was not found, we are still compiling
         //write the received data to the page
         serverData.innerHTML = event.data;
      } else { // string found, compilation done
         //write the received data to the page, but without "Listening..." string
         // extract computer name and port number
         var name_port = event.data.substr(s+32).split(" ");
         name = "client.php?name=" + name_port[0];
         port = "&port=" + name_port[2] + "&m";
         serverData.innerHTML = event.data.substring(0,s) + "<h4>Listening:</h4>\n";
         // register GUI event handlers
         cmd.addEventListener("keyup", command);
         window.onbeforeunload = exit_spike;
         browse.onclick = exit_spike;
         upload.onclick = exit_spike;
         // make GUI appear on page
         eSource.removeEventListener("message", sse_listener);
      }
   }
}

//check for browser support
if(typeof(EventSource)!=="undefined") {
        //create an object, passing it the name and location of the server side script
        eSource = new EventSource("send_sse.php?dir=" + path + "&proc=" + process);
	eSource.onmessage = sse_listener;
} else serverData.innerHTML= "Whoops! Your browser does not receive server-sent events.";


// JTAG client request for spike debug
var spikeReq = new XMLHttpRequest();

function spikeReqListener() {
  sout.innerHTML = this.responseText;                
}
spikeReq.onload = spikeReqListener;

function command(event) {
    if (event.key === "Enter") {
       spikeReq.open("get", name + port + "&data=" + cmd.value);
       spikeReq.send();
    }
}

function exit_spike() {
    spikeReq.open("get", name + port + "&data=q");
    spikeReq.send();
    serverData.innerHTML = "";
    sout.innerHTML = "";
    cmd.style.border = "transparent";
    cmd.value = "";
}

