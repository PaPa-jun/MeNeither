// 定义默认选项
const DEFAULT_OPTIONS = {
    indentSize: '1.2em',
    commentDelimiter: '//',
    lineNumber: true,
    lineNumberPunc: ':',
    noEnd: false,
    captionCount: undefined,
    scopeLines: true
};

// 渲染伪代码函数
function renderPseudocode() {
    // 选择所有具有.pseudocode类的元素
    let pseudocodes = document.querySelectorAll(".pseudocode");

    pseudocodes.forEach((pse) => {
        // 获取原始文本并清空HTML内容以防重复渲染
        let code = pse.textContent;
        pse.innerHTML = '';
        // 使用pseudocode.js库进行渲染
        pseudocode.render(code, pse, DEFAULT_OPTIONS);
    });
}

document.addEventListener('DOMContentLoaded', function () {
    // 页面首次加载时渲染伪代码
    renderPseudocode();
});