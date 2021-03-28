let name = "";  // computer name string for GET request to JTAG client
let fpga = "";  // FPGA number string for GET request to JTAG client
var si;  // periodic interval timer for GUI refresh
var eSource;  // event dsource for synthesizer output update

function sse_listener(event) {
   if(name == "") {
      var r = event.data.indexOf("<h4>Agora digite: ./remote ");
      if( r == -1 ) { // if the "Agora..." string was not found, we are still synthesizing 
         //write the received data to the page
         document.getElementById("serverData").innerHTML = event.data;
      } else {
         //write the received data to the page, but without "Agora..." string
         document.getElementById("serverData").innerHTML = event.data.substring(0,r) + "<br>\n";
         // extract computer name and FPGA number
         var name_fpga = event.data.substr(r+27).split(" ");
         name = "client.php?name=" + name_fpga[0];
         fpga = "&fpga=" + name_fpga[1];
         // register GUI event handlers
         swi7.onpointerdown = swi7_click;
         swi6.onpointerdown = swi6_click;
         swi5.onpointerdown = swi5_click;
         swi4.onpointerdown = swi4_click;
         swi3.onpointerdown = swi3_click;
         swi2.onpointerdown = swi2_click;
         swi1.onpointerdown = swi1_click;
         swi0.onpointerdown = swi0_click;
         si = setInterval(update, 1000);
         window.onbeforeunload = exit_fpga;
         browse.onclick = exit_fpga;
         upload.onclick = exit_fpga;
         // make GUI appear on page
         swi0.src = "components/switchOff.png";
         swi1.src = "components/switchOff.png";
         swi2.src = "components/switchOff.png";
         swi3.src = "components/switchOff.png";
         swi4.src = "components/switchOff.png";
         swi5.src = "components/switchOff.png";
         swi6.src = "components/switchOff.png";
         swi7.src = "components/switchOff.png";
         seg.src = "components/segmentsDisplay.png";
         led0.src = "components/ledOff.png";
         led1.src = "components/ledOff.png";
         led2.src = "components/ledOff.png";
         led3.src = "components/ledOff.png";
         led4.src = "components/ledOff.png";
         led5.src = "components/ledOff.png";
         led6.src = "components/ledOff.png";
         led7.src = "components/ledOff.png";
         eSource.removeEventListener("message", sse_listener);
      }
   }
}

//check for browser support
if(typeof(EventSource)!=="undefined") {
        //create an object, passing it the name and location of the server side script
        eSource = new EventSource("send_sse.php?dir=" + path);
	eSource.onmessage = sse_listener;
} else document.getElementById("serverData").innerHTML=
          "Whoops! Your browser does not receive server-sent events.";

function ledSegReqListener() {
 if(this.responseText.charCodeAt(0) != 0x53) { // first character is not S (error)
  let r = Number("0x" + this.responseText);
  if(r&0x0001) led0.src = "components/ledOn.png";
  else         led0.src = "components/ledOff.png";
  if(r&0x0002) led1.src = "components/ledOn.png";
  else         led1.src = "components/ledOff.png";
  if(r&0x0004) led2.src = "components/ledOn.png";
  else         led2.src = "components/ledOff.png";
  if(r&0x0008) led3.src = "components/ledOn.png";
  else         led3.src = "components/ledOff.png";
  if(r&0x0010) led4.src = "components/ledOn.png";
  else         led4.src = "components/ledOff.png";
  if(r&0x0020) led5.src = "components/ledOn.png";
  else         led5.src = "components/ledOff.png";
  if(r&0x0040) led6.src = "components/ledOn.png";
  else         led6.src = "components/ledOff.png";
  if(r&0x0080) led7.src = "components/ledOn.png";
  else         led7.src = "components/ledOff.png";
  if(r&0x0100) seg0.src = "components/seg0.png";
  else         seg0.src = "components/segNada.png";
  if(r&0x0200) seg1.src = "components/seg1.png";
  else         seg1.src = "components/segNada.png";
  if(r&0x0400) seg2.src = "components/seg2.png";
  else         seg2.src = "components/segNada.png";
  if(r&0x0800) seg3.src = "components/seg3.png";
  else         seg3.src = "components/segNada.png";
  if(r&0x1000) seg4.src = "components/seg4.png";
  else         seg4.src = "components/segNada.png";
  if(r&0x2000) seg5.src = "components/seg5.png";
  else         seg5.src = "components/segNada.png";
  if(r&0x4000) seg6.src = "components/seg6.png";
  else         seg6.src = "components/segNada.png";
  if(r&0x8000) seg7.src = "components/seg7.png";
  else         seg7.src = "components/segNada.png";
  if(display == "LCD") document.getElementById("LCD").innerHTML = "1234";
 }
}

var ledSegReq = new XMLHttpRequest();
ledSegReq.onload = ledSegReqListener;

let s0 = false;
let s1 = false;
let s2 = false;
let s3 = false;
let s4 = false;
let s5 = false;
let s6 = false;
let s7 = false;

function swi7_click(event) {
  s7 = !s7;
  if(s7) swi7.src = "components/switchOn.png";
  else   swi7.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100111" + (s7 ? "1" : "0") );
  ledSegReq.send();
}

function swi6_click(event) {
  s6 = !s6;
  if(s6) swi6.src = "components/switchOn.png";
  else   swi6.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100110" + (s6 ? "1" : "0") );
  ledSegReq.send();
}

function swi5_click(event) {
  s5 = !s5;
  if(s5) swi5.src = "components/switchOn.png";
  else   swi5.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100101" + (s5 ? "1" : "0") );
  ledSegReq.send();
}

function swi4_click(event) {
  s4 = !s4;
  if(s4) swi4.src = "components/switchOn.png";
  else   swi4.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100100" + (s4 ? "1" : "0") );
  ledSegReq.send();
}

function swi3_click(event) {
  s3 = !s3;
  if(s3) swi3.src = "components/switchOn.png";
  else   swi3.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100011" + (s3 ? "1" : "0") );
  ledSegReq.send();
}

function swi2_click(event) {
  s2 = !s2;
  if(s2) swi2.src = "components/switchOn.png";
  else   swi2.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100010" + (s2 ? "1" : "0") );
  ledSegReq.send();
}

function swi1_click(event) {
  s1 = !s1;
  if(s1) swi1.src = "components/switchOn.png";
  else   swi1.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100001" + (s1 ? "1" : "0") );
  ledSegReq.send();
}

function swi0_click(event) {
  s0 = !s0;
  if(s0) swi0.src = "components/switchOn.png";
  else   swi0.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + fpga + "&data=0100000" + (s0 ? "1" : "0") );
  ledSegReq.send();
}

function update() {
  ledSegReq.open("get", name + fpga + "&data=00100000" );
  ledSegReq.send();
}

function exit_fpga(event) {
  ledSegReq.open("get", name + fpga + "&data=exit" );
  ledSegReq.send();
  clearInterval(si);
  swi0.src = "components/switchNada.png";
  swi1.src = "components/switchNada.png";
  swi2.src = "components/switchNada.png";
  swi3.src = "components/switchNada.png";
  swi4.src = "components/switchNada.png";
  swi5.src = "components/switchNada.png";
  swi6.src = "components/switchNada.png";
  swi7.src = "components/switchNada.png";
  seg.src  = "components/segNada.png";
  seg0.src = "components/segNada.png";
  seg1.src = "components/segNada.png";
  seg2.src = "components/segNada.png";
  seg3.src = "components/segNada.png";
  seg4.src = "components/segNada.png";
  seg5.src = "components/segNada.png";
  seg6.src = "components/segNada.png";
  seg7.src = "components/segNada.png";
  led0.src = "components/ledNada.png";
  led1.src = "components/ledNada.png";
  led2.src = "components/ledNada.png";
  led3.src = "components/ledNada.png";
  led4.src = "components/ledNada.png";
  led5.src = "components/ledNada.png";
  led6.src = "components/ledNada.png";
  led7.src = "components/ledNada.png";
  document.getElementById("LCD").innerHTML = "";
}

