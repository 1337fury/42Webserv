const card_back = document.querySelectorAll(".container .box .box_inner .box_back");
const box = document.querySelectorAll(".container .box .box_inner .box_front");

const oArr = [
    "images/glove.png",
    "images/chair.png",
    "images/grip.png",
    "images/hand-mirror.png",
    "images/lamp.png",
    "images/love.png",
    "images/soap.png",
    "images/tattoo-artist.png",
    "images/clipboard.png",
    "images/adhesives.png",
    "images/glove.png",
    "images/chair.png",
    "images/grip.png",
    "images/hand-mirror.png",
    "images/lamp.png",
    "images/love.png",
    "images/soap.png",
    "images/tattoo-artist.png",
    "images/clipboard.png",
    "images/adhesives.png"
];

function add_image(box, arr) {
    let i;
    i = 0;
    box.forEach(ele => {
        let img = document.createElement('img');
        img.src = arr[i];
        ele.appendChild(img);
        ele.parentElement.parentElement.style.order = Math.floor(Math.random() * arr.length);
        i++;
    }); 
};

add_image(card_back, oArr);

function comp(cliked)
{
    let active_boxs = document.querySelectorAll(".container .box.active");
    active_boxs.forEach(el => {
        if(!(cliked.nextElementSibling.firstElementChild.src == el.firstElementChild.lastElementChild.firstElementChild.src)) {
            el.classList.remove("active");
            cliked.parentElement.parentElement.classList.remove("clicked");
            document.querySelector(".container").style.pointerEvents = "auto";
        }
        else {
            el.classList.remove("active");
            el.classList.add("done");
            cliked.parentElement.parentElement.classList.remove("clicked");
            cliked.parentElement.parentElement.classList.add("done");
            document.querySelector(".container").style.pointerEvents = "auto";
        };
    });
};

box.forEach(e => {
    e.addEventListener("click", () => {
        let n;
        n = 0;
        if (!e.parentElement.parentElement.classList.contains('active'))
        e.parentElement.parentElement.classList.add('active');
        box.forEach(element => {
            if (element.parentElement.parentElement.classList.contains('active'))
                n++;
        });
        if (n == 2) {
            document.querySelector(".container").style.pointerEvents = "none";
            e.parentElement.parentElement.classList.remove('active');
            e.parentElement.parentElement.classList.add("clicked");
            setTimeout(() => {
                comp(e);
            }, "500");
        };
    });
});
    
