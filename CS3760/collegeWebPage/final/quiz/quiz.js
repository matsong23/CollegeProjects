const form = document.getElementById("quiz")
form.addEventListener('submit', function(event) {
    event.preventDefault()

    const name = form.q1.value 
    const q2 = form.q2.value
    const q3 = form.q3.value
    const q4 = Array.from(form.q4).filter(v => v.checked).map(v => v.value)
    const q5 = form.q5.value
    
    const res = document.getElementById("result")
    res.innerHTML = ''

    const t = []
    var answered = 0;
    if (q2) {
        answered++
        const a = document.createElement('p')
        if (q2 === "No") {
            a.textContent = "You answered question 1 correct!"
        } else {
            a.textContent = "Each line in a transaction can only have 1 account."
        }
        t.push(a)
    }
    if (q3) {
        answered++
        const a = document.createElement('p')
        if (q2 === "No") {
            a.textContent = "You answered question 2 correct!"
        } else {
            a.textContent = "A line in a transaction can only have either a debit or a credit amount."
        }
        t.push(a)
    }
    if (q4.length > 0) {
        answered++
        const a = document.createElement('p')
        if (q4.length === 2 && q4.includes("CorD") && q4.includes("acct")) {
            a.textContent = "You answered question 3 correct!"
        } else {
            a.textContent = "Each line must have a debit or a credit amount, each line must have an account, all lines in a transaction cannot be just debits or just credits, and each transaction must have at least 2."
        }
        t.push(a)
    }
    if (q5) {
        answered++
        const a = document.createElement('p')
        if (q5 === "-600") {
            a.textContent = "You answered question 4 correct!"
        } else {
            a.textContent = "When retained earnings is applied after the end of the fiscal year the balance of every account is treated as $0."
        }
        t.push(a)
    }

    const p = document.createElement('p')
    p.textContent = answered > 0 ? `Thank you ${name} for answering the questions!` : `Why didn't you answer the questions ${name}?`
    res.appendChild(p)
    t.forEach(ans => res.appendChild(ans))
    const br = document.getElementById("bres")
    br.classList.remove("none")
})
