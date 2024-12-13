let pseudocodes = document.querySelectorAll(".pseudocode");

let options = {
    lineNumber: true
};

pseudocodes.forEach((pse) => {
    let code = pse.textContent;
    pse.innerHTML = '';
    pseudocode.render(code, pse, options);
});