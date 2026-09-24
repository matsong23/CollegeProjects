const playPauseBtn = document.querySelector(".play-pause");
const stopBtn = document.querySelector(".stop");
const rwdBtn = document.querySelector(".rwd");
const fwdBtn = document.querySelector(".fwd");
const timeLabel = document.querySelector(".time");
const ccBtn = document.querySelector(".sub");
const player = document.querySelector("video");

var canvas = document.getElementById("myCanvas");
var drawer = canvas.getContext("2d");

const shapes = [];

clearCanvas = () => {
    drawer.clearRect(0, 0, canvas.width, canvas.height);
    shapes.length = 0;
};

randomize = () => {
    for (shape of shapes) {
        shape.x = Math.random() * canvas.width;
        shape.y = Math.random() * canvas.height;
    }

    drawer.clearRect(0, 0, canvas.width, canvas.height);
    for (shape of shapes) {
        if (shape.shape === "square") {
            drawer.fillStyle = shape.color;
            drawer.fillRect(shape.x, shape.y, shape.length, shape.length);
        }
        if (shape.shape === "circle") {
            drawer.beginPath();
            drawer.arc(shape.x, shape.y, shape.r, 0, 2 * Math.PI);
            drawer.fillStyle = shape.color;
            drawer.strokeStyle = shape.color;
            drawer.fill();
            drawer.stroke();
            drawer.closePath();
        }
        if (shape.shape === "rectanlge") {
            drawer.fillStyle = shape.color;
            drawer.fillRect(shape.x, shape.y, shape.length, shape.width);
        }
    }
}

addSquare = () => {
    var x = Math.random() * canvas.width;
    var y = Math.random() * canvas.height;
    var color = `#${Math.floor(Math.random()*16777215).toString(16)}`;
    drawer.fillStyle = color;
    var length = 10 + Math.random() * 40;
    drawer.fillRect(x, y, length, length);
    shapes.push({shape: "square", x: x, y: y, color: color, length: length});
};

addCircle = () => {
    var x = Math.random() * canvas.width;
    var y = Math.random() * canvas.height;
    var color = `#${Math.floor(Math.random()*16777215).toString(16)}`;
    var r = 5 + Math.random() * 30;
    drawer.beginPath();
    drawer.arc(x, y, r, 0, 2 * Math.PI);
    drawer.fillStyle = color;
    drawer.strokeStyle = color;
    drawer.fill();
    drawer.stroke();
    drawer.closePath();
    shapes.push({shape: "circle", x: x, y: y, color: color, r: r});
};

addRectangle = () => {
    var x = Math.random() * canvas.width;
    var y = Math.random() * canvas.height;
    var color = `#${Math.floor(Math.random()*16777215).toString(16)}`;
    drawer.fillStyle = color;
    var length = 10 + Math.random() * 40;
    var width = 10 + Math.random() * 40;
    drawer.fillRect(x, y, length, width);
    shapes.push({shape: "rectanlge", x: x, y: y, color: color, length: length, width: width});
};

player.removeAttribute("controls");

playPauseBtn.onclick = () => {
  if (player.paused) {
    player.play();
    playPauseBtn.textContent = "Pause";
  } else {
    player.pause();
    playPauseBtn.textContent = "Play";
  }
};

stopBtn.onclick = () => {
  player.pause();
  player.currentTime = 0;
  playPauseBtn.textContent = "Play";
};

rwdBtn.onclick = () => {
  player.currentTime -= 3;
};

fwdBtn.onclick = () => {
  player.currentTime += 3;
  if (player.currentTime >= player.duration || player.paused) {
    player.pause();
    player.currentTime = 0;
    playPauseBtn.textContent = "Play";
  }
};

player.ontimeupdate = () => {
  const minutes = Math.floor(player.currentTime / 60);
  const seconds = Math.floor(player.currentTime - minutes * 60);
  const minuteValue = minutes < 10 ? `0${minutes}` : minutes;
  const secondValue = seconds < 10 ? `0${seconds}` : seconds;

  const mediaTime = `${minuteValue}:${secondValue}`;
  timeLabel.textContent = mediaTime;
};

ccBtn.onclick = () => {
  if (player.textTracks[0].mode === "showing") {
        player.textTracks[0].mode = "hidden";
        ccBtn.classList.remove("active");
        ccBtn.textContent = "TURN CC ON";
    } else {
        player.textTracks[0].mode = "showing";
        ccBtn.classList.add("active");
        ccBtn.textContent = "TURN CC OFF";
  }
};