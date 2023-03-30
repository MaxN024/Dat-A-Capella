const button = document.querySelector('.container-group');
var audioElement = new Audio("Audio/ClipCombi.wav");


button.addEventListener('click', (e) => {
  audioElement.play()
  let overlay = document.createElement('span');
  let left = e.clientX - button.getBoundingClientRect().left;
  let top = e.clientY- button.getBoundingClientRect().top;
  overlay.style.cssText = `
    position: absolute;
    height: 350px;
    width: 350px;
    top: ${top}px;
    left: ${left}px;
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
  button.appendChild(overlay);
});