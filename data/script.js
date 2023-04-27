(function () {
    'use strict';

    const page = document.querySelector('#page');;
    const servoCount = 8;

    let sendPostCatch = {};
    const btnArry = ["Lower", "Raise", "Walk", "Forw/Rev"]

    document.body.addEventListener('input', e => {
        const elem = e.target;
        const { name, arg1 } = elem.dataset;

        Object.assign(sendPostCatch, { [name + arg1]: [name, arg1, elem.value] });
    });

    document.body.addEventListener('click', e => {
        const elem = e.target;
        const { name, arg1, arg2 } = elem.dataset;

        if (elem.type === 'button') {
            sendVal(name, arg1, arg2);

            if (name === 'servo' || name === 'speed') {
                elem.parentNode.children[0].value = arg2;
            }
        }
    });

    (function createSpeedElem() {
        const conainer = document.createElement('div');
        conainer.className = 'inputContainer speed';

        conainer.innerHTML = `
            Speed
            <input type="range" min="1" max="20" id="servo" data-name="speed" data-arg1="">
            <button type="button" data-name="speed" data-arg2="10">Slow</button>
            <button type="button" data-name="speed" data-arg2="15">Medium</button>
            <button type="button" data-name="speed" data-arg2="20">Fast</button>
            `;

        page.append(conainer);
    }());

    (function createServoElem() {
        for (let i = 0; i < servoCount; i++) {
            const conainer = document.createElement('div');
            conainer.className = 'inputContainer';

            conainer.innerHTML = `
            Servo ${i + 1}
            <input type="range" min="0" max="180" id="servo" data-name="servo" data-arg1=${i}>
            <button type="button" data-name="servo" data-arg1=${i} data-arg2="0">Start</button>
            <button type="button" data-name="servo" data-arg1=${i} data-arg2="90">Middle</button>
            <button type="button" data-name="servo" data-arg1=${i} data-arg2="180">End</button>
            `;

            page.append(conainer);
        }
    }());


    (function createCustomBtnElem() {
        const box = document.createElement('div');
        box.className = 'box';

        btnArry.forEach((i, index) => {
            const j = document.createElement('div');
            j.innerHTML = `
            <button class="boxBtn" data-name=custom data-arg1=${index} data-arg2="" type="button">${i}</button>
            `
            box.append(j);
        })
        page.append(box);
    }());

    function sendVal(name, val1, val2) {
        console.log(name, val1, val2)
        fetch(`/updateData?output=${name}&state=${val1}&state2=${val2}`);
        // fetch(`http://robot.local/updateData?output=${name}&state=${val1}&state2=${val2}`);
    }

    setInterval(() => {
        if (Object.getOwnPropertyNames(sendPostCatch).length !== 0) {
            Object.keys(sendPostCatch).forEach(key => {
                const item = sendPostCatch[key];
                sendVal(item[0], item[1], item[2]);
            });
            sendPostCatch = {};
        }
    }, 200);
})();