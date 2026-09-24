const transactions = [];

/**
 * Reads in the csv file and creates tables based on the data.
 */
function loadCSV() {
    const table = document.getElementById("transactions-tables")
    if (table.childElementCount > 0) return // Already run

    fetch('transactions.csv')
        .then(response => response.text())
        .then(data => {
            const rows = data.split(/\r?\n|\r/).slice(1); 
            rows.forEach(row => { 
                const [acctName, acctType, debit, credit, transactionNumber] = row.split(',');
                const d = Number(debit)
                const c = Number(credit)
                if (transactions.find(t => t.transactionNumber === transactionNumber)) {
                    const t = transactions.find(t => t.transactionNumber === transactionNumber);
                    t.lines.push({
                        acctName: acctName,
                        acctType: acctType.toLocaleUpperCase(),
                        debit: d / 100,     // Convert from cents to dollars
                        credit: c / 100     // Convert from cents to dollars
                    });
                } else {
                    const transaction = {
                        transactionNumber: transactionNumber,
                        lines: [{
                            acctName: acctName,
                            acctType: acctType.toLocaleUpperCase(),
                            debit: d / 100, // Convert from cents to dollars
                            credit: c / 100 // Convert from cents to dollars
                        }]
                    };
                    transactions.push(transaction);
                }
            });
            display();
        })
        .catch(error => console.error('Error loading CSV:', error));
}

/**
 * Parent function for creating the transaction tables and account table
 */
function display() {
    displayTransactions();
    displayBreakdown();
}

/**
 * Creates a tables for all transactions
 */
function displayTransactions() {
    const tableLoc = document.getElementById('transactions-tables')
    const theader = document.getElementById("t-header")
    theader.classList.remove("none")

    for (const t of transactions) {
        const div = document.createElement('div')
        div.classList.add("transaction-table")
        const table = document.createElement('table')
        const hrow = document.createElement('tr')
        const hlineNum = document.createElement('th')
        hlineNum.textContent = "Line"
        const haccName = document.createElement('th')
        haccName.textContent = "Account"
        const haccType = document.createElement('th')
        haccType.textContent = "Account Type"
        const hdebit = document.createElement('th')
        hdebit.textContent = "Debit"
        const hcredit = document.createElement('th')
        hcredit.textContent = "Credit"

        hrow.appendChild(hlineNum)
        hrow.appendChild(haccName)
        hrow.appendChild(haccType)
        hrow.appendChild(hdebit)
        hrow.appendChild(hcredit)
        table.appendChild(hrow)

        var i = 1
        for (const l of t.lines) {
            const drow = document.createElement('tr')
            const clineNum = document.createElement('td')
            clineNum.textContent = i
            const caccName = document.createElement('td')
            caccName.textContent = l.acctName
            const caccType = document.createElement('td')
            caccType.textContent = l.acctType
            const cdebit = document.createElement('td')
            cdebit.textContent = (l.debit === 0 ? '' : l.debit)
            const ccredit = document.createElement('td')
            ccredit.textContent = (l.credit === 0 ? '' : l.credit)

            drow.appendChild(clineNum)
            drow.appendChild(caccName)
            drow.appendChild(caccType)
            drow.appendChild(cdebit)
            drow.appendChild(ccredit)
            table.appendChild(drow)
            i++;
        }
        const h3 = document.createElement('h3')
        h3.textContent = `Transaction #${t.transactionNumber}`
        div.appendChild(h3)
        const isValid = isTransactionValid(t)
        if (isValid === 1) {
            div.appendChild(createP("Invalid transaction. Debits and credits don't match"))
        } else if (isValid === 2) {
            div.appendChild(createP("Invalid transaction. A transaction line can not have both a debit and a credit amount"))
        } else if (isValid === 3) {
            div.appendChild(createP("Invalid transaction. Each line must have a debit or a credit"))
        } else if (isValid === 4) {
            div.appendChild(createP("Invalid transaction. Each line must have an account and its type"))
        }
        div.appendChild(table)
        tableLoc.appendChild(div)
    }
}

/**
 * Creates an account table from the transactions array showing the 
 * credit total, debit total, and final balance of all accounts in 
 * valid transactions
 */
function displayBreakdown() {
    // Get account breakdown
    const accts = [];
    transactions.forEach(t => {
        const isValid = isTransactionValid(t)
        if (isValid !== 0) return
        t.lines.forEach(line => {
            const acct = accts.find(a => a.acctName === line.acctName);
            if (acct !== undefined) {
                acct.debit += line.debit;
                acct.credit += line.credit;
            } else {
                accts.push({
                    acctName: line.acctName,
                    acctType: line.acctType,
                    debit: line.debit,
                    credit: line.credit
                });
            }
        });
    });

    // Construct table
    const table = document.createElement('table')

    const hrow = document.createElement('tr')
    const hname = document.createElement('th')
    hname.textContent = "Account Name"
    const htype = document.createElement('th')
    htype.textContent = "Account Type"
    const hdebit = document.createElement('th')
    hdebit.textContent = "Debit Total"
    const hcredit = document.createElement('th')
    hcredit.textContent = "Credit Total"
    const hbal = document.createElement('th')
    hbal.textContent = "End Balance"
    
    hrow.appendChild(hname)
    hrow.appendChild(htype)
    hrow.appendChild(hdebit)
    hrow.appendChild(hcredit)
    hrow.appendChild(hbal)
    table.appendChild(hrow)

    for (const acct of accts) {
        const drow = document.createElement('tr')
        const cname = document.createElement('td')
        cname.textContent = acct.acctName
        const ctype = document.createElement('td')
        ctype.textContent = acct.acctType
        const cdebit = document.createElement('td')
        cdebit.textContent = acct.debit
        const ccredit = document.createElement('td')
        ccredit.textContent = acct.credit
        const cbal = document.createElement('td')
        cbal.textContent = acct.debit - acct.credit

        drow.appendChild(cname)
        drow.appendChild(ctype)
        drow.appendChild(cdebit)
        drow.appendChild(ccredit)
        drow.appendChild(cbal)
        table.appendChild(drow)
    }
    const tableLoc = document.getElementById('accounts-table')
    const h2 = document.createElement('h2')
    h2.textContent = "Breakdown of Accounts"
    tableLoc.appendChild(h2)
    tableLoc.appendChild(table)
}

/**
 * Verify that all lines have either a credit or a debit total,
 * account name, and account type.
 * Credit total and debit total must match.
 * @param {transaction to validate} t 
 * @returns 0 if tranaction is valid, otherwise the number coresponding to why it is invalid
 */
function isTransactionValid(t) {
    const creditTotal = t.lines.reduce((total, line) => {
        const c = Number(line.credit);
        return total + (c !== NaN ? c : 0);
    }, 0);
    const debitTotal = t.lines.reduce((total, line) => {
        const d = Number(line.debit);
        return total + (d !== NaN ? d : 0);
    }, 0);

    if (creditTotal != debitTotal) return 1
    for (const line of t.lines) {
        if (line.credit > 0 && line.debit > 0) return 2
        if (!line.credit && !line.debit) return 3
        if (!line.acctName || !line.acctType) return 4
    }
    return 0
}


function createP(text) {
    const p = document.createElement('p')
    p.textContent = text
    return p
}