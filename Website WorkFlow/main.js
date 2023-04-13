const buttonsend = document.querySelector('#send');
const buttonA = document.querySelector('#A');
const buttonB = document.querySelector('#B');
const buttonC = document.querySelector('#C');
// const instrumentbutton = document.querySelector('.dropdown-content');
const hihatbutton = document.querySelector('#drop1');
const clapbutton = document.querySelector('#drop2');
const clavebutton = document.querySelector('#drop3');
const ridebutton = document.querySelector('#drop4');
const kickbutton = document.querySelector('#drop5');
var Hihat = new Audio("Hihat.wav");
var Kick = new Audio("Kick1.wav");
var Clap = new Audio("Clap.wav");
var Ride = new Audio("Ride1.wav");
var Clave = new Audio("Clave1.wav");
var instrument = new Audio();
var instrument2 = new Audio();
var instrument3 = new Audio();
var vibrationD4 = 1;
var vibration3 = 1;
var vibration9C = 1;
let buttonAtoggle = false;
let buttonBtoggle = false;
let buttonCtoggle = false;

/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function myFunction() {
  document.getElementById("myDropdown").classList.toggle("show");
}

//when user selects certain instrument change variable instrument
function HihatFunction() {
  console.log("hello world1");
  instrument = Hihat;
}
function ClapFunction() {
  console.log("hello world2");
  instrument = Clap;
}
function ClaveFunction() {
  console.log("hello world3");
  instrument = Clave;
}
function RideFunction() {
  console.log("hello world4");
  instrument = Ride;
}
function KickFunction() {
  console.log("hello world5");
  instrument = Kick;
}


// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event2) {
  if (!event2.target.matches('.dropbtn')) {
    var dropdowns = document.getElementsByClassName("dropdown-content");
    var i;
    for (i = 0; i < dropdowns.length; i++) {
      var openDropdown = dropdowns[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
}

/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function myFunction2() {
  document.getElementById("myDropdown2").classList.toggle("show");
}

//when user selects certain instrument change variable instrument
function HihatFunction2() {
  console.log("hello world6");
  instrument2 = Hihat;
}
function ClapFunction2() {
  console.log("hello world7");
  instrument2 = Clap;
}
function ClaveFunction2() {
  console.log("hello world8");
  instrument2 = Clave;
}
function RideFunction2() {
  console.log("hello world9");
  instrument2 = Ride;
}
function KickFunction2() {
  console.log("hello world10");
  instrument2 = Kick;
}


// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event12) {
  if (!event12.target.matches('.dropbtn2')) {
    var dropdowns2 = document.getElementsByClassName("dropdown-content2");
    var j;
    for (j = 0; j < dropdowns2.length; j++) {
      var openDropdown2 = dropdowns[j];
      if (openDropdown2.classList.contains('show')) {
        openDropdown2.classList.remove('show');
      }
    }
  }
}

/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function myFunction3() {
  document.getElementById("myDropdown3").classList.toggle("show");
}

//when user selects certain instrument change variable instrument
function HihatFunction3() {
  console.log("hello world11");
  instrument3 = Hihat;
}
function ClapFunction3() {
  console.log("hello world12");
  instrument3 = Clap;
}
function ClaveFunction3() {
  console.log("hello world13");
  instrument3 = Clave;
}
function RideFunction3() {
  console.log("hello world14");
  instrument3 = Ride;
}
function KickFunction3() {
  console.log("hello world15");
  instrument3 = Kick;
}


// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event22) {
  if (!event22.target.matches('.dropbtn3')) {
    var dropdowns3 = document.getElementsByClassName("dropdown-content3");
    var l;
    for (l = 0; l < dropdowns3.length; l++) {
      var openDropdown3 = dropdowns3[l];
      if (openDropdown3.classList.contains('show')) {
        openDropdown3.classList.remove('show');
      }
    }
  }
}


//click on sendbutton makes sound and visual
buttonsend.addEventListener('click', (e) => {
  let overlay = document.createElement('span');
  let left = e.clientX - buttonsend.getBoundingClientRect().left;
  let top = e.clientY- buttonsend.getBoundingClientRect().top;
  overlay.style.cssText = `
    position: absolute;
    height: 350px;
    width: 350px;
    top: ${top+20}px;
    left: ${left+20}px;
    transform: translate(-50%, -50%);
    background-color: crimson;
    z-index: -1;
    border-radius: 50%;
    opacity: 0;
    animation: button_animation 1s ease;
  `;
overlay.addEventListener('animationend', (e) => {
  e.target.remove();
});
  buttonsend.appendChild(overlay);
});


  //OOCSI
  document.addEventListener("DOMContentLoaded", (sender) => {
    // initialize OOCSI and connect
    OOCSI.connect("wss://oocsi.id.tue.nl/ws");
    // subscribe to OOCSI channel for incoming data
    OOCSI.subscribe("pencilsenderweb", (sender) => {
      // data received from channel is written to the content of the "dataDisplay" element
      document.querySelector("#dataDisplay").innerText = sender.data.position;
    });
    // register click handler for button: click sends data to OOCSI channel
       document.querySelector(".sendbutton").addEventListener("click", (sender) => {
       OOCSI.send("pencilsenderweb", { send: 1 });
      });
  });

   //product C9 (ButtonB) listener
   document.addEventListener("DOMContentLoaded", (eventoocsi) => {
    // initialize OOCSI and connect
    // OOCSI.connect("wss://oocsi.id.tue.nl/ws");

    OOCSI.subscribe("pencilcaseCommunication2", function (msg) {
//      var pressure3 = msg.data.d9C_listen_3;
//      var pressure4 = msg.data.d9C_listen_D4;
      vibration9C = msg.data.vib_9C;
      // console.log("9C (ProductB) =", vibration9C);
    });
    OOCSI.subscribe("pencilcaseCommunication1", function (msg) {
      // var pressure1 = msg.data.D4_listen_3;
      // var pressure2 = msg.data.D4_listen_9C;
      vibrationD4 = msg.data.vib_D4;
      // console.log("D4 (ProductA) =", vibrationD4);
     });
     OOCSI.subscribe("pencilcaseCommunication3", function (msg) {
      //      var pressure5 = msg.data.d3_listen_D4;
      //      var pressure6 = msg.data.d3_listen_9C;
            vibration3 = msg.data.vib_3;
            // console.log("3 (ProductC) =", vibration3);
      console.log("D4 (ProductA) =", vibrationD4, ",", "9C (ProductB) =", vibration9C, "," , "3 (ProductC) =", vibration3);
      });
 });


//click on buttonA toggles playing vibdata as selected instrument
  buttonA.addEventListener('click', (e1) => { 
    e1.stopPropagation(); 
    if (!buttonAtoggle) {
      buttonAtoggle = true;
      console.log("succes!", buttonAtoggle);
      myLoop();
     } else {
       buttonAtoggle = false;
       console.log("zekers!", buttonAtoggle);
    } 
  });

  //click on buttonB toggles playing vibdata as selected instrument
  buttonB.addEventListener('click', (e2) => { 
    e2.stopPropagation(); 
    if (!buttonBtoggle) {
      buttonBtoggle = true;
      console.log("succes!", buttonBtoggle);
      myLoop2();
     } else {
       buttonBtoggle = false;
       console.log("zekers!", buttonBtoggle);
    } 
  });

  //click on buttonB toggles playing vibdata as selected instrument
  buttonC.addEventListener('click', (e3) => { 
    e3.stopPropagation(); 
    if (!buttonCtoggle) {
      buttonCtoggle = true;
      console.log("succes!", buttonCtoggle);
      myLoop3();
     } else {
       buttonCtoggle = false;
       console.log("zekers!", buttonCtoggle);
    } 
  });



  
  function myLoop() {         
    setTimeout(function() { 
        if(vibrationD4 == 1){
        instrument.play();  
        console.log(vibrationD4); 
      } else{
        console.log(vibrationD4);
      }      
      myLoop();                                     
    }, 1000)
  }   


  function myLoop2() {         
    setTimeout(function() { 
        if(vibration9C == 1){
        instrument2.play();  
        console.log(vibration9C); 
      } else{
        console.log(vibration9C);
      }      
      myLoop2();                                     
    }, 1000)
  }   


  function myLoop3() {         
    setTimeout(function() { 
        if(vibration3 == 1){
        instrument3.play();  
        console.log(vibration3); 
      } else{
        console.log(vibration3);
      }      
      myLoop3();                                     
    }, 1000)
  }   
