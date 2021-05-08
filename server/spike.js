let name = "";  // computer name string for GET request to JTAG client
let port = "";  // port number string for GET request to JTAG client
var eSource;  // event dsource for synthesizer output update
var initial = true;

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
         cmd.onkeyup = command;
         window.onbeforeunload = exit_spike;
         browse.onclick = exit_spike;
         upload.onclick = exit_spike;
         server_HTML_replace(event);
      }
   } else {
       if (port == "") serverData.innerHTML = "";  // connection was terminated
       else {
	  if(initial) {
             pcReq.open("get", name + port + "&data=pc 0");
             pcReq.send();
             initial = false;
          }
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
   serverData.innerHTML = event.data.replace(/Listening for debug commands on [^.]+.<br>/,
            "<h4 style=\"font-family:serif\">pronto para receber comandos na caixa azul:</h4>");
}

// the requests to spike are sent in the following sequence:
// user command, pc, reg, mem
// Each request response triggers the next request in the sequence.
// The user command request is triggered by user input.
// The first request is for pc, so the register window is already populated
// when the user entgers his first command.

// request for pc display (first line of register window)
var pcReq = new XMLHttpRequest();
var pc = "";
function pcReqListener() {
   if(this.responseText[0] != 'S') {
      pc = this.responseText;      
      regReq.open("get", name + port + "&data=reg 0");
      regReq.send();
   } else {
       reg.innerHTML = "";
       mem.innerHTML = "";
   }
}
pcReq.onload = pcReqListener;

// request for register display
var regReq = new XMLHttpRequest();
function regReqListener() {
  reg.innerHTML = "&nbsp;  pc: " + pc + "<br>" + this.responseText;
  if (memory.length != 0) {
     memReq.open("get", name + port + "&data=mem " + memory[0]);
     memReq.send();
  }
}
regReq.onload = regReqListener;

// request for memory display
var memReq = new XMLHttpRequest();
var memory = [];
var memory_idx = 0;
var memHTML = "";
function memReqListener() {
   if (memory_idx > 0) memHTML = memHTML + "<br>";
   memHTML =  memHTML + memory[memory_idx++] + ": " + this.responseText;
   if (memory_idx < memory.length) {
      memReq.open("get", name + port + "&data=mem " + memory[memory_idx]);
      memReq.send();
   } else {
      mem.innerHTML = memHTML;
      memory_idx = 0;
      memHTML = "";   
   }
}
memReq.onload = memReqListener;

// request for user command response
var spikeReq = new XMLHttpRequest();
function spikeReqListener() {
  sout.innerHTML = this.responseText;
  pcReq.open("get", name + port + "&data=pc 0");
  pcReq.send();
}
spikeReq.onload = spikeReqListener;

function command(event) {
    if (event.key === "Enter") {
       if (cmd.value.split(" ")[0] == "mem") {
          memory[memory.length] = cmd.value.split(" ")[1];
          memory_idx = 0;
          memReq.open("get", name + port + "&data=mem " + memory[0]);
          memReq.send();
       } else {
          spikeReq.open("get", name + port + "&data=" + cmd.value);
          spikeReq.send();
       }    
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
    memReq.onload = nada;
    cmd.onkeyup = nada_e;
    browse.onclick = nada_e;
    port = "";  // mark connection as terminated
    sout.innerHTML = "";
    cmd.style.border = "transparent";
    cmd.value = "";
    sout.style.border = "transparent";
    sout.innerHTML = "";
    reg.style.border = "transparent";
    reg.innerHTML = "";
    mem.style.border = "transparent";
    mem.innerHTML = "";
    help.innerHTML = "";
    serverData.innerHTML = "";
    initial = true;
}

