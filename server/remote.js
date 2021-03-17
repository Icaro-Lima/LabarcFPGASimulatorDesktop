let name = "";
let fpga = "";

function sse_listener(event) {
   if(name == "") {
      var r = event.data.indexOf("<h4>Agora digite: ./remote ");
      if( r == -1 ) {
         //write the received data to the page
         document.getElementById("serverData").innerHTML = event.data;
      } else {
         var name_fpga = event.data.substr(r+27).split(" ");
         document.getElementById("serverData").innerHTML = event.data.substring(0,r)
                                                + "<h4>Agora pode usar aqui: </h4>";
         name = name_fpga[0];
         fpga = name_fpga[1];
         swi7.onpointerdown = swi7_click;
         swi6.onpointerdown = swi6_click;
         swi5.onpointerdown = swi5_click;
         swi4.onpointerdown = swi4_click;
         swi3.onpointerdown = swi3_click;
         swi2.onpointerdown = swi2_click;
         swi1.onpointerdown = swi1_click;
         swi0.onpointerdown = swi0_click;
         setInterval(update, 1000);
         window.onbeforeunload = exit_fpga;
      }
   }
}

//check for browser support
if(typeof(EventSource)!=="undefined") {
        //create an object, passing it the name and location of the server side script
        var eSource = new EventSource("send_sse.php?dir=" + path);
	eSource.onmessage = sse_listener;
} else document.getElementById("serverData").innerHTML=
          "Whoops! Your browser does not receive server-sent events.";

function reqListener() {
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
  else         seg0.src = "components/nada.png";
  if(r&0x0200) seg1.src = "components/seg1.png";
  else         seg1.src = "components/nada.png";
  if(r&0x0400) seg2.src = "components/seg2.png";
  else         seg2.src = "components/nada.png";
  if(r&0x0800) seg3.src = "components/seg3.png";
  else         seg3.src = "components/nada.png";
  if(r&0x1000) seg4.src = "components/seg4.png";
  else         seg4.src = "components/nada.png";
  if(r&0x2000) seg5.src = "components/seg5.png";
  else         seg5.src = "components/nada.png";
  if(r&0x4000) seg6.src = "components/seg6.png";
  else         seg6.src = "components/nada.png";
  if(r&0x8000) seg7.src = "components/seg7.png";
  else         seg7.src = "components/nada.png";
}

var oReq = new XMLHttpRequest();
oReq.onload = reqListener;

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
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100111" + (s7 ? "1" : "0") );
  oReq.send();
}

function swi6_click(event) {
  s6 = !s6;
  if(s6) swi6.src = "components/switchOn.png";
  else   swi6.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100110" + (s6 ? "1" : "0") );
  oReq.send();
}

function swi5_click(event) {
  s5 = !s5;
  if(s5) swi5.src = "components/switchOn.png";
  else   swi5.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100101" + (s5 ? "1" : "0") );
  oReq.send();
}

function swi4_click(event) {
  s4 = !s4;
  if(s4) swi4.src = "components/switchOn.png";
  else   swi4.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100100" + (s4 ? "1" : "0") );
  oReq.send();
}

function swi3_click(event) {
  s3 = !s3;
  if(s3) swi3.src = "components/switchOn.png";
  else   swi3.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100011" + (s3 ? "1" : "0") );
  oReq.send();
}

function swi2_click(event) {
  s2 = !s2;
  if(s2) swi2.src = "components/switchOn.png";
  else   swi2.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100010" + (s2 ? "1" : "0") );
  oReq.send();
}

function swi1_click(event) {
  s1 = !s1;
  if(s1) swi1.src = "components/switchOn.png";
  else   swi1.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100001" + (s1 ? "1" : "0") );
  oReq.send();
}

function swi0_click(event) {
  s0 = !s0;
  if(s0) swi0.src = "components/switchOn.png";
  else   swi0.src = "components/switchOff.png"; 
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=0100000" + (s0 ? "1" : "0") );
  oReq.send();
}

function update() {
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=00100000" );
  oReq.send();
}

function exit_fpga(event) {
  oReq.open("get", "http://lad.dsc.ufcg.edu.br/hdl/client.php?name="
	           + name + "&fpga=" + fpga + "&data=exit" );
  oReq.send();
}

