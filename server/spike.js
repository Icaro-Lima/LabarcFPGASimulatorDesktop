let name = "";  // computer name string for GET request to JTAG client
let port = "";  // port number string for GET request to JTAG client
var eSource;  // event dsource for synthesizer output update

function sse_listener(event) {
   if(name == "") {
      var s = event.data.indexOf("Listening for debug commands on ");
      if( s == -1 ) { // if the "Listening..." string was not found, we are still compiling
         //write the received data to the page
         serverData.innerHTML = event.data;
      } else { // string found, compilation done
         // extract computer name and port number
         var name_port = event.data.substr(s+32).split(" ");
         name = "client.php?name=" + name_port[0];
         port = "&port=" + name_port[2] + "&m";
         // register GUI event handlers
         cmd.addEventListener("keyup", command);	      
         window.onbeforeunload = exit_spike;
         browse.onclick = exit_spike;
         upload.onclick = exit_spike;
         server_HTML_replace(event);
      }
   } else {
       if (port == "") serverData.innerHTML = "";  // connection was terminated
       else {
          regReq.open("get", name + port + "&data=reg 0");
          regReq.send();
          server_HTML_replace(event);
       }
   }
}

//check for browser support
if(typeof(EventSource)!=="undefined") {
        //create an object, passing it the name and location of the server side script
        eSource = new EventSource("send_sse.php?dir=" + path + "&proc=" + process);
	eSource.onmessage = sse_listener;
} else serverData.innerHTML= "Whoops! Your browser does not receive server-sent events.";

//write the received data to the page, but without "Listening..." string
function server_HTML_replace(event) {
   serverData.innerHTML = event.data.replace(/Listening for debug commands on [^.]+./,
                                       "<h4>Pronto para receber comandos na caixa azul:</h4>");
}

// request for register display
var regReq = new XMLHttpRequest();

function regReqListener() {
  reg.innerHTML = this.responseText;            
}
regReq.onload = regReqListener;

// request for spike debug command from user
var spikeReq = new XMLHttpRequest();

function spikeReqListener() {
  sout.innerHTML = this.responseText;
  regReq.open("get", name + port + "&data=reg 0");
  regReq.send();
}
spikeReq.onload = spikeReqListener;

function command(event) {
    if (event.key === "Enter") {
       spikeReq.open("get", name + port + "&data=" + cmd.value);
       spikeReq.send();
    }
}

function nada() { }
function nada_e(event) { }

function exit_spike() {
    eSource.removeEventListener("message", sse_listener);
    spikeReq.open("get", name + port + "&data=q");
    spikeReq.send();
    spikeReq.onload = nada;
    regReq.onload = nada;
    cmd.addEventListener("keyup", nada_e);
    browse.onclick = nada_e;
    port = "";  // mark connection as terminated
    sout.innerHTML = "";
    cmd.style.border = "transparent";
    cmd.value = "";
    sout.style.border = "transparent";
    sout.innerHTML = "";
    reg.style.border = "transparent";
    reg.innerHTML = "";
    serverData.innerHTML = "";
}

