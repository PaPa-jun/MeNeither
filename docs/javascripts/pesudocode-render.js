let pseudocodes = document.querySelectorAll(".pseudocode");

var DEFAULT_OPTIONS = {
    indentSize: '1.2em',
    commentDelimiter: '//',
    lineNumber: true,
    lineNumberPunc: ':',
    noEnd: false,
    captionCount: undefined,
    scopeLines: true
};

pseudocodes.forEach((pse) => {
    let code = pse.textContent;
    pse.innerHTML = '';
    pseudocode.render(code, pse, DEFAULT_OPTIONS);
});