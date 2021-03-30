let name = "";  // computer name string for GET request to JTAG client
let port = "";  // port number string for GET request to JTAG client
var si;  // periodic interval timer for GUI refresh
var eSource;  // event dsource for synthesizer output update

function sse_listener(event) {
   if(name == "") {
      var r = event.data.indexOf("<h4>Agora digite: ./remote ");
      if( r == -1 ) { // if the "Agora..." string was not found, we are still synthesizing 
         //write the received data to the page
         serverData.innerHTML = event.data;
      } else { // string found, FPGA is configured and JTAG server was started
         //write the received data to the page, but without "Agora..." string
         serverData.innerHTML = event.data.substring(0,r) + "<br>\n";
         // extract computer name and FPGA number
         var name_port = event.data.substr(r+27).split(" ");
         name = "client.php?name=" + name_port[0];
         port = "&port=" + name_port[1];
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
         window.onbeforeunload = exit_jtag;
         browse.onclick = exit_jtag;
         upload.onclick = exit_jtag;
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
         if (display == "RISC") { 
            risc1.innerHTML = "&nbsp; pc &nbsp; &nbsp; &nbsp; &nbsp; instruction " +
                              "&nbsp; &nbsp; &nbsp;&nbsp; WriteData MemWrite Branch";
            risc2.innerHTML = "SrcA &nbsp; &nbsp; SrcB &nbsp; ALUResult " +
                              "Result ReadData MemtoReg RegWrite";
            x0.innerHTML = "x0&nbsp; zero: ";
            x4.innerHTML = "x4&nbsp; tp &nbsp;: ";
            x8.innerHTML = "x8&nbsp; s0 &nbsp;: ";
            xc.innerHTML = "x12 a2 &nbsp;: ";
            x1.innerHTML = " &nbsp;ra : ";                
            x2.innerHTML = " &nbsp;sp : ";                
            x3.innerHTML = " &nbsp;gp : ";                
            x5.innerHTML = " &nbsp;t0 : ";                
            x6.innerHTML = " &nbsp;t1 : ";                
            x7.innerHTML = " &nbsp;t2 : ";                
            x9.innerHTML = " &nbsp;s1 : ";                
            xa.innerHTML = " &nbsp;a0 : ";                
            xb.innerHTML = " &nbsp;a1 : ";                
            xd.innerHTML = " &nbsp;a3 : ";                
            xe.innerHTML = " &nbsp;a4 : ";                
            xf.innerHTML = " &nbsp;a5 : ";                
         }
         eSource.removeEventListener("message", sse_listener);
      }
   }
}


//check for browser support
if(typeof(EventSource)!=="undefined") {
        //create an object, passing it the name and location of the server side script
        eSource = new EventSource("send_sse.php?dir=" + path);
	eSource.onmessage = sse_listener;
} else serverData.innerHTML= "Whoops! Your browser does not receive server-sent events.";


// JTAG client request for RISC-V registers
var riscRegReq = new XMLHttpRequest();

function riscRegReqListener() {
 if(this.responseText.charCodeAt(0) != 0x53) { // first character is not S (error)
  var r = this.responseText.toUpperCase();
  r0.innerHTML = r.substr(0,2);
  r1.innerHTML = r.substr(2,2);
  r2.innerHTML = r.substr(4,2);
  r3.innerHTML = r.substr(6,2);
  r4.innerHTML = r.substr(8,2);
  r5.innerHTML = r.substr(10,2);
  r6.innerHTML = r.substr(12,2);
  r7.innerHTML = r.substr(14,2);
  r8.innerHTML = r.substr(16,2);
  r9.innerHTML = r.substr(18,2);
  ra.innerHTML = r.substr(20,2);
  rb.innerHTML = r.substr(22,2);
  rc.innerHTML = r.substr(24,2);
  rd.innerHTML = r.substr(26,2);
  re.innerHTML = r.substr(28,2);
  rf.innerHTML = r.substr(30,2);
 }
}
riscRegReq.onload = riscRegReqListener;

// JTAG client request for RISC-V LCD
var riscLcdReq = new XMLHttpRequest();

function riscLcdReqListener() {
 if(this.responseText.charCodeAt(0) != 0x53) { // first character is not S (error)
  var r = this.responseText.toUpperCase();
  let f = Number("0x" + r.substr(22,2));         
  LCD.innerHTML =
     //       PC               instruction           WriteData
     r.substr( 0,2) + " " + r.substr( 2,8) + " " + r.substr(18,2) + 
     //      MemWrite           Branch
     ( f&0x01 ? "*" : "_" ) + ( f&0x02 ? "*" : "_" ) + "<br>" +
     //       SrcA              SrcB                ALUResult
     r.substr(10,2) + " " + r.substr(12,2) + " " + r.substr(14,2) + " " + 
     //       Result            ReadData
     r.substr(16,2) + " " + r.substr(20,2) + 
     //       Memto Reg         RegWrite
     ( f&0x04 ? "*" : "_" ) + ( f&0x08 ? "*" : "_" );
  riscRegReq.open("get", name + port + "&data=00000000" );
  riscRegReq.send();
 }
}
riscLcdReq.onload = riscLcdReqListener;

// JTAG client request for LCD
var lcdReq = new XMLHttpRequest();

function lcdReqListener() {
 if(this.responseText.charCodeAt(0) != 0x53) { // first character is not S (error)
  var r = this.responseText.toUpperCase();
  //                 lcd_a                    lcd_b
  LCD.innerHTML = r.slice(-16) + "<br>" + r.slice(0,16);
 }
}
lcdReq.onload = lcdReqListener;

// JTAG client request for LED and SEG
var ledSegReq = new XMLHttpRequest();

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
  if(display == "LCD") {
      lcdReq.open("get", name + port + "&data=00110000" );
      lcdReq.send();
  } else if (display == "RISC") {
      riscLcdReq.open("get", name + port + "&data=00100011" );
      riscLcdReq.send();
  }
 }
}
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
  ledSegReq.open("get", name + port + "&data=0100111" + (s7 ? "1" : "0") );
  ledSegReq.send();
}

function swi6_click(event) {
  s6 = !s6;
  if(s6) swi6.src = "components/switchOn.png";
  else   swi6.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100110" + (s6 ? "1" : "0") );
  ledSegReq.send();
}

function swi5_click(event) {
  s5 = !s5;
  if(s5) swi5.src = "components/switchOn.png";
  else   swi5.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100101" + (s5 ? "1" : "0") );
  ledSegReq.send();
}

function swi4_click(event) {
  s4 = !s4;
  if(s4) swi4.src = "components/switchOn.png";
  else   swi4.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100100" + (s4 ? "1" : "0") );
  ledSegReq.send();
}

function swi3_click(event) {
  s3 = !s3;
  if(s3) swi3.src = "components/switchOn.png";
  else   swi3.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100011" + (s3 ? "1" : "0") );
  ledSegReq.send();
}

function swi2_click(event) {
  s2 = !s2;
  if(s2) swi2.src = "components/switchOn.png";
  else   swi2.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100010" + (s2 ? "1" : "0") );
  ledSegReq.send();
}

function swi1_click(event) {
  s1 = !s1;
  if(s1) swi1.src = "components/switchOn.png";
  else   swi1.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100001" + (s1 ? "1" : "0") );
  ledSegReq.send();
}

function swi0_click(event) {
  s0 = !s0;
  if(s0) swi0.src = "components/switchOn.png";
  else   swi0.src = "components/switchOff.png"; 
  ledSegReq.open("get", name + port + "&data=0100000" + (s0 ? "1" : "0") );
  ledSegReq.send();
}

function update() {
  ledSegReq.open("get", name + port + "&data=00100000" );
  ledSegReq.send();
}

function nada() { }
function nada_e(event) { }

function exit_jtag(event) {
  swi7.onpointerdown = nada_e;
  swi6.onpointerdown = nada_e;
  swi5.onpointerdown = nada_e;
  swi4.onpointerdown = nada_e;
  swi3.onpointerdown = nada_e;
  swi2.onpointerdown = nada_e;
  swi1.onpointerdown = nada_e;
  swi0.onpointerdown = nada_e;
  browse.onclick = nada_e;
  upload.onclick = nada_e;
  lcdReq.onload = nada;
  riscLcdReq.onload = nada;
  riscRegReq.onload = nada;
  ledSegReq.onload = nada;
  ledSegReq.open("get", name + port + "&data=exit" );
  ledSegReq.send();
  clearInterval(si);
  // make GUI disappear from page
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
  LCD.innerHTML = "";
  risc1.innerHTML = "";
  risc2.innerHTML = "";
  x0.innerHTML = "";
  x4.innerHTML = "";
  x8.innerHTML = "";
  xc.innerHTML = "";
  x1.innerHTML = "";                
  x2.innerHTML = "";                
  x3.innerHTML = "";                
  x5.innerHTML = "";                
  x6.innerHTML = "";                
  x7.innerHTML = "";                
  x9.innerHTML = "";                
  xa.innerHTML = "";                
  xb.innerHTML = "";                
  xd.innerHTML = "";                
  xe.innerHTML = "";                
  xf.innerHTML = "";                
  r0.innerHTML = "";
  r4.innerHTML = "";
  r8.innerHTML = "";
  rc.innerHTML = "";
  r1.innerHTML = "";                
  r2.innerHTML = "";                
  r3.innerHTML = "";                
  r5.innerHTML = "";                
  r6.innerHTML = "";                
  r7.innerHTML = "";                
  r9.innerHTML = "";                
  ra.innerHTML = "";                
  rb.innerHTML = "";                
  rd.innerHTML = "";                
  re.innerHTML = "";                
  rf.innerHTML = "";                
}

