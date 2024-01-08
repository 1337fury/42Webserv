let search_btn = document.getElementById("search-btn");
let search_form = document.querySelector("form.search-form");
search_btn.onclick = function () {
    search_form.classList.toggle('active');
    menu_box.classList.remove('active');
    shoping_card.classList.remove('active');
    login_form.classList.remove('active');
}


let menu_btn = document.getElementById("menu-btn");
let menu_box = document.querySelector(".header .nav");
menu_btn.onclick = function () {
    menu_box.classList.toggle('active');
    search_form.classList.remove('active');
    shoping_card.classList.remove('active');
    login_form.classList.remove('active');
}

let shoping_btn = document.getElementById("shoping-btn");
let shoping_card = document.querySelector(".header .shoping-card");
shoping_btn.onclick = function () {
    shoping_card.classList.toggle('active');
    search_form.classList.remove('active');
    menu_box.classList.remove('active');
    login_form.classList.remove('active');
}

let user_btn = document.getElementById("user-btn");
let login_form = document.querySelector(".header .login_form");
user_btn.onclick = function () {
    login_form.classList.toggle('active');
    search_form.classList.remove('active');
    menu_box.classList.remove('active');
    shoping_card.classList.remove('active');
}

window.onscroll = function () {
    search_form.classList.remove('active');
    menu_box.classList.remove('active');
    shoping_card.classList.remove('active');
    login_form.classList.remove('active');
}



var swiper = new Swiper(".product_slider", {
    loop: true,
    spaceBetween: 20,
    autoplay: {
        delay: 7500,
        disableOnteraction: false,
    },
    centerSlides: true,
    breakpoints: {
      0: {
        slidesPerView: 1,
      },
      768: {
        slidesPerView: 2,
      },
      1020: {
        slidesPerView: 3,
      },
    },
  });


  var swiper = new Swiper(".customers_slider", {
    loop: true,
    spaceBetween: 20,
    autoplay: {
        delay: 7500,
        disableOnteraction: false,
    },
    centerSlides: true,
    breakpoints: {
      0: {
        slidesPerView: 1,
      },
      768: {
        slidesPerView: 2,
      },
      1020: {
        slidesPerView: 3,
      },
    },
  });