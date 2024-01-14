#!/usr/bin/env node

console.log("Content-type: text/html\r\n\r\n");

console.log(`
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>CGI Artwork Animation</title>
  <style>
    canvas {
      border: 1px solid #000;
    }
  </style>
</head>
<body>
  <canvas id="myCanvas" width="400" height="300"></canvas>
  <script>
`);

console.log(`
    const canvas = document.getElementById('myCanvas');
    const ctx = canvas.getContext('2d');
    
    const circles = [];

    function Circle(x, y, radius, dx, dy, color) {
      this.x = x;
      this.y = y;
      this.radius = radius;
      this.dx = dx;
      this.dy = dy;
      this.color = color;
    }

    Circle.prototype.draw = function () {
      ctx.beginPath();
      ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
      ctx.fillStyle = this.color;
      ctx.fill();
      ctx.closePath();
    };

    Circle.prototype.update = function () {
      if (this.x + this.radius > canvas.width || this.x - this.radius < 0) {
        this.dx = -this.dx;
      }

      if (this.y + this.radius > canvas.height || this.y - this.radius < 0) {
        this.dy = -this.dy;
      }

      this.x += this.dx;
      this.y += this.dy;
    };

    function animate() {
      requestAnimationFrame(animate);
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      circles.forEach(circle => {
        circle.draw();
        circle.update();
      });
    }

    // Create circles
    for (let i = 0; i < 3; i++) {
      const radius = 30;
      const x = Math.random() * (canvas.width - radius * 2) + radius;
      const y = Math.random() * (canvas.height - radius * 2) + radius;
      const dx = (Math.random() - 0.5) * 2; // Random velocity
      const dy = (Math.random() - 0.5) * 2;
      const color = \`rgb(\${Math.random() * 255}, \${Math.random() * 255}, \${Math.random() * 255})\`;

      circles.push(new Circle(x, y, radius, dx, dy, color));
    }

    animate();
  </script>
</body>
</html>
`);
