const brightness = document.querySelector(".brightness");
const saturation  = document.querySelector(".saturation");
const inversion  = document.querySelector(".inversion");
const grayscale  = document.querySelector(".grayscale");

let Brightness = {
    min : 0,
    max : 200
};
let Saturation = {
    min : 0,
    max : 250
};
let Inversion = {
    min : 0,
    max : 100
}; 
let Grayscale = {
    min : 0,
    max : 100
};

let range = document.querySelector("#range");
let value = document.querySelector(".value");
let image = document.querySelector(".image img");

range.min = Brightness.min;
range.max = Brightness.max;

let brigh = 100, sat = 100, inver = 0, gray = 0;
let rotate = 0, hFlip = 1, vFlip = 1;

range.value = brigh;
value.textContent = `${range.value}%`;



range.onchange = () => { 
    const active =         document.querySelector("ul.first li   button.active");
    value.textContent = `${range.value}%`;
    image.style.filter = `brightness(${brigh}%) saturate(${sat}%) invert(${inver}%) grayscale(${gray}%)`;
     if (active.getAttribute('data-name') =="brightness") {
            brigh = range.value;
        }
        else if (active.getAttribute('data-name') == "saturate") {
            sat = range.value;
        }
        else if (active.getAttribute('data-name') == "invert") {
            inver = range.value;
        }
        else if (active.getAttribute('data-name') == "grayscale") {
            gray = range.value;
        }
};

document.querySelectorAll(".filter_options ul.first li button").forEach((ele) => {
    ele.onclick = () => {
        document.querySelectorAll(".filter_options ul.first li button").forEach((ele) => {
            ele.classList.remove("active");
        });
        ele.classList.add("active");
        document.querySelector(".filter_options .opt_value .head .name").textContent = ele.textContent;
        if (ele.classList.contains("brightness")) {
            range.min = Brightness.min;
            range.max = Brightness.max;
            range.value = brigh;
        }
        else if (ele.classList.contains("saturation")) {
            range.min = Saturation.min;
            range.max = Saturation.max;
            range.value = sat;
        }
        else if (ele.classList.contains("inversion")) {
            range.min = Inversion.min;
            range.max = Inversion.max;
            range.value = inver;
        }
        else if (ele.classList.contains("grayscale")) {
            range.min = Grayscale.min;
            range.max = Grayscale.max;
            range.value = gray;
        }
        value.textContent = `${range.value}%`;
    }
});


const other_options = document.querySelectorAll(".other_options ul.second li button");

other_options.forEach(ele => {
   ele.addEventListener("click", () => {
       if (ele.getAttribute('data-name') =="rotate") {
            rotate += 90;
        }
        else if (ele.getAttribute('data-name') == "rRotate") {
            rotate -= 90;
        }
        else if (ele.getAttribute('data-name') == "hFlip") {
            hFlip *= -1;
        }
        else if (ele.getAttribute('data-name') == "vFlip") {
            vFlip *= -1;
        }
        image.style.transform = `rotate(${rotate}deg) scaleX(${hFlip}) scaleY(${vFlip})`;
    });
});


document.getElementById("reset").onclick = function () {
const active =         document.querySelector("ul.first li   button.active");
    image.style.filter = `brightness(100%) saturate(100%) invert(0) grayscale(0)`;
    image.style.transform = `rotate(0) scaleX(1) scaleY(1)`;
    brigh = 100, sat = 100, inver = 0, gray = 0;
    if (active.getAttribute("data-name") == "invert" || active.getAttribute("data-name") == "grayscale") {
        range.value = inver;
    } else {
        range.value = sat;
    }
    value.textContent = `${range.value}%`;
}
