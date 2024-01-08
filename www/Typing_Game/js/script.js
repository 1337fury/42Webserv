/*
  Advices
  - Always Check The Console
  - Take Your Time To Name The Identifiers
  - DRY

  Steps To Create The Project
  [01] Create HTML Markup
  [02] Add Styling And Separate From Logic
  [03] Create The App Logic
  ---- [01] Add Levels
  ---- [02] Show Level And Seconds
  ---- [03] Add Array Of Words
  ---- [04] ِAdd Start Game Button
  ---- [05] Generate Upcoming Words
  ---- [06] Disable Copy Word And Paste Event + Focus On Input
  ---- [07] Start Play Function
  ---- [08] Start The Time And Count Score
  ---- [09] Add The Error And Success Messages
  [04] Your Trainings To Add Features
  ---- [01] Save Score To Local Storage With Date
  ---- [02] Choose Levels From Select Box
  ---- [03] Break The Logic To More Functions
  ---- [04] Choose Array Of Words For Every Level
  ---- [05] Write Game Instruction With Dynamic Values
  ---- [06] Add 3 Seconds For The First Word
*/

// Array Of Words
let words = [
    "Hello",
    "Programming",
    "Code",
    "Javascript",
    "Town",
    "Country",
    "Testing",
    "Youtube",
    "Linkedin",
    "Twitter",
    "Github",
    "Leetcode",
    "Internet",
    "Python",
    "Scala",
    "Destructuring",
    "Paradigm",
    "Styling",
    "Cascade",
    "Documentation",
    "Coding",
    "Funny",
    "Working",
    "Dependencies",
    "Task",
    "Runner",
    "Roles",
    "Test",
    "Rust",
    "Playing"
  ];

  let Select = document.getElementById('levels');

  // Setting Levels
  const levels = {
    "Easy": 7,
    "Normal": 5,
    "Hard": 2
  };

  let defaultLevelName = Select.value;
  let defaultLevelSeconds = levels[defaultLevelName];

  //Catch Selectors

  let lvl = document.querySelector('.lvl');
  let seconds = document.querySelector('.seconds');
  let start = document.querySelector('.start');
  let the_word = document.querySelector('.the-word');
  let input = document.querySelector('.input');
  let upcoming_words = document.querySelector('.upcoming-words');
  let time = document.querySelector('.time span');
  let got = document.querySelector('.got');
  let total = document.querySelector('.total');
  let finish = document.querySelector('.finish');

  Select.onchange = () => {
    defaultLevelName = Select.value;
     defaultLevelSeconds = levels[defaultLevelName]
     lvl.innerHTML = Select.value;
    seconds.innerHTML = defaultLevelSeconds;
    time.innerHTML = defaultLevelSeconds;
  }


  lvl.innerHTML = defaultLevelName;
  seconds.innerHTML = defaultLevelSeconds;
  time.innerHTML = defaultLevelSeconds;
  total.innerHTML = words.length;

  input.onpaste = function() {
    return false;
  }

  start.onclick = function () {
    //Remove the playing button
    this.remove();
    //focus on the input
    input.focus();
    //Remove Select Option
    Select.remove();
    //call generate word function
    genWords();
  }

  function genWords() {
    
    let random_word = words[Math.floor(Math.random() * words.length)];
    let word_index = words.indexOf(random_word);
    
    //Remove the word from arrray 
    words.splice(word_index, 1);
    
    //show the random word in the_word div
    the_word.innerHTML = random_word;

    //empty the upcoming word div
    upcoming_words.innerHTML = '';
    
    //add words in the upcoming word div
    words.forEach(word => {
      let word_div = document.createElement('div');
      word_div.append(word);
      upcoming_words.appendChild(word_div);
    });
    
    startGame();
   
  }


  function startGame() {

    //Restart the counter
    time.innerHTML = defaultLevelSeconds;

    const counter = setInterval(function() {

      time.innerHTML--;
  
      if(time.innerHTML == '0') {
        //stop the counter
        clearInterval(counter);

        if(input.value.toLowerCase() == the_word.innerHTML.toLowerCase()) {
          
          //Empty the input field
          input.value = '';
          //Increase Score
          got.innerHTML++;
          //show another word
          if(words.length > 0) {
            genWords();
          } else {
            let good = document.createElement('div');
            good.className = "good";
            good.append("Congratulation");
            finish.appendChild(good);
          }
          
        } else {
          //create element div
          let bad = document.createElement('div');
          //add class name
          bad.className = "bad";
          //append text node
          bad.append("Game Over !");
          // append the div to the finish div 
          finish.appendChild(bad);
        }
      }
  
    }, 1000);
  }
  

//  let friends = ['Houssin', 'ayoub', 'Azdine', 'ihab', 'Youssef'];
 
//  let count = 0;
//  let i;
//  for(i=0; i < friends.length; i++) {
//   friends[i].charAt(0) == friends[i].charAt(0).toUpperCase() ? console.log(friends[i]) : count += 1;
//  }
//  console.log('friends printed and ignored names count is ' + count);