//Get Elements
let next = document.querySelector(".container .footer button.submit");
let qNumber = document.querySelector(".qNumber");
let qCounter = document.querySelector(".qCounter");
let question = document.querySelector(".question");
let answers = document.querySelector(".answers");
let minutes = document.querySelector(".minutes");
let seconds = document.querySelector(".seconds");


let right_answer =  0;

function getQuestions() {
    let myQuestions = new XMLHttpRequest();

    myQuestions.onreadystatechange = function () {

        if(this.readyState === 4 && this.status === 200) {
            //convert the JSON response text to Object js format
            let myObj = JSON.parse(this.responseText);
            //build the timer function
            var timer = setInterval(() => {
                seconds.innerHTML = (Number(seconds.innerHTML) + 1);
                if (seconds.innerHTML < 10) seconds.innerHTML = '0' + seconds.innerHTML;
                if (seconds.innerHTML == 60) {
                    seconds.innerHTML = 0;
                    minutes.innerHTML++;
                    if (minutes.innerHTML < 10) minutes.innerHTML = '0' + minutes.innerHTML;
                }
            }, 1000);
            //set the number of questions to qCounter html area
            qCounter.innerHTML = myObj.length;
            //start the current index with 0
            let i = 0;
            //call the getContent function : this function get content from myObj Object and working with it 
            getContent(question, answers, myObj, i);
            //add click event to the next button 
            next.addEventListener('click', () => {
                //if the number is 10(max) the timer will stopped
                if (qNumber.innerHTML == 10) clearInterval(timer);
                //search for the checked radio input and compare his value with the correct answer from MyObj variable
                document.querySelectorAll("input[type='radio']").forEach(ele => {
                    if(ele.checked === true) 
                        if(i < myObj.length && ele.value === myObj[i]['correct_answer']) right_answer++;
                })
                //increment the index for get next question
                i++;
                //get next question and their answers with call getContent function after increment the current index by 1
                getContent(question, answers, myObj, i);
            });
        }
    }
    //Get questions from JSON file 
    myQuestions.open("GET", "Questions.json", true);
    myQuestions.send();
}
getQuestions();

function getContent(question, answersbox, obj, i) {
    //start this block only if the current index < object length
    if(i < obj.length) {
        //add the question to its html area
        question.innerHTML = obj[i].title;
        //filter the specific answers from the object  
        let specificAnswers = Object.keys(obj[i]).filter(ele => {
            return ele.startsWith('answer');
        });
        //remove all children of answer box before add the new elements to it
        answersbox.innerHTML ='';
        //add the new elements to the answersbox div
        for(let j = 0; j < specificAnswers.length; j++) {
            let box = document.createElement('div');
            box.classList = 'box';
            var radiobox = document.createElement('input');
            radiobox.type = 'radio';
            radiobox.name = 'answer';
            radiobox.value = obj[i][specificAnswers[j]];
            radiobox.id = specificAnswers[j];
            if(j==0) radiobox.checked = true;
        
            let label = document.createElement('label')
            label.htmlFor = specificAnswers[j];
            label.innerHTML = obj[i][specificAnswers[j]];

            box.appendChild(radiobox);
            box.appendChild(label);

            answersbox.appendChild(box);
        }
        qNumber.innerHTML++;
    }
    if(i === obj.length)
    {
        document.body.removeChild(document.body.firstElementChild);
        let resultsEle = document.createElement("div");
        resultsEle.className = "results";

        let p = document.createElement("p");
        p.innerHTML = `You get <span>${right_answer}/${obj.length}</span>`;
        
        let btn = document.createElement("button");
        btn.id = "restart";
        btn.type = "submit";
        btn.innerHTML = "Restart";
        
        resultsEle.appendChild(p);
        resultsEle.appendChild(btn);
        
        document.body.appendChild(resultsEle);

        if(right_answer > 8)
            document.querySelector(".results p span").className = "great";
        else if(right_answer  > 5)
            document.querySelector(".results p span").className = "Not_bad";
        else if(right_answer <= 5)
            document.querySelector(".results p span").className = "bad";
        
        document.getElementById("restart").onclick = () => location.reload();
    }
}
