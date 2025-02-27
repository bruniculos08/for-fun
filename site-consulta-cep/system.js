function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function clearInputCEP() {
    document.getElementById("texto-cep").value = "";
}

async function showResult(cep) {
    var saved = localStorage.getItem("CEP=" + String(cep));
    var found;
    if (saved != null) {
        found = JSON.parse(saved);
    }
    else {
        found = await pesquisacep(cep);
        // if ("error" in found) {
        //     console.log("line 18");
        //     return;
        // }
    }
    if (document.getElementById('menu-pesquisa').style.display === "block")
        closeDiv("menu-pesquisa");
    var result = document.getElementById('menu-resultado');
    if (result.style.display === "block")
        await (closeDiv("menu-resultado"));
    result.style.display = "block";
    document.getElementById("cep-pesquisado-info").innerHTML = String(cep);
    if (!("error" in found)) {
        document.getElementById('logradouro-info').innerHTML = String(found.logradouro);
        document.getElementById('bairro-info').innerHTML = String(found.bairro);
        document.getElementById('localidade-info').innerHTML = String(found.localidade);
        document.getElementById('uf-info').innerHTML = String(found.uf);
        document.getElementById('ibge-info').innerHTML = String(found.ibge);
    }
}

async function submitCEP(opt, cep) {
    if (opt == 0) {
        var input = document.getElementById("texto-cep");
        const input_value = input.value;
        input.value = "";
        await (showResult(input_value));
    }
    else if (opt == 1) {
        var input = document.getElementById("texto-cep2");
        const input_value = input.value;
        input.value = "";
        await (showResult(input_value));
    }
    else {
        await (showResult(cep));
    }
}

async function closeDiv(arg) {
    var divbox = document.getElementById(arg);
    divbox.style.animation = "animate-to-top 2s";
    await (sleep(1900));
    divbox.style.animation = "animate-to-middle 2s";
    divbox.style.display = "none";
}

// function meu_callback(conteudo) {
//     if (!("erro" in conteudo)) {

//         var logradouro_info = document.getElementById('rua-info');
//         var bairro_info = document.getElementById('bairro-info');
//         var logradouro_info = document.getElementById('logradouro-info');
//         var uf_info = document.getElementById('uf-info');
//         var ibge_info = document.getElementById('ibge-info');

//         // Atualiza os campos com os valores.
//         logradouro_info.innerHTML = (conteudo.logradouro);
//         bairro_info.innerHTML = (conteudo.bairro);
//         logradouro_info.innerHTML = (conteudo.localidade);
//         uf_info.innerHTML = (conteudo.uf);
//         ibge_info.innerHTML = (conteudo.ibge);

//         var cep = conteudo.cep;
//         var found = new CEP_info(cep.innerHTML, logradouro_info.innerHTML, bairro_info.innerHTML, logradouro_info.innerHTML, uf_info.innerHTML, ibge_info.innerHTML);
//         localStorage.setItem("CEP=" + String(cep), JSON.stringify(found));
//     }
//     else {
//         // CEP não Encontrado.
//         // limpa_formulário_cep();
//         alert("CEP não encontrado.");
//     }
// }

async function pesquisacep(valor) {

    // Nova variável "cep" somente com dígitos.
    var cep = valor.replace(/\D/g, '');

    // Verifica se campo cep possui valor informado.
    if (cep != "") {

        // Expressão regular para validar o CEP.
        var validacep = /^[0-9]{8}$/;

        // Valida o formato do CEP.
        if (validacep.test(cep)) {

            var logradouro_info = document.getElementById('logradouro-info');
            var bairro_info = document.getElementById('bairro-info');
            var localidade_info = document.getElementById('localidade-info');
            var uf_info = document.getElementById('uf-info');
            var ibge_info = document.getElementById('ibge-info');

            // Preenche os campos com "..." enquanto consulta webservice.
            logradouro_info.innerHTML = "...";
            bairro_info.innerHTML = "...";
            localidade_info.innerHTML = "...";
            uf_info.innerHTML = "...";
            ibge_info.innerHTML = "...";

            var obj = await (await (fetch('https://viacep.com.br/ws/' + cep + '/json/'))).json();
            // console.log(JSON.stringify(obj));

            if (!("erro" in obj)) {
                localStorage.setItem("CEP=" + String(cep), JSON.stringify(obj));
                return obj;
            }
            alert("CEP não encontrado!");
        }
        else {
            // CEP é inválido.
            // limpa_formulário_cep();
            alert("Formato de CEP inválido.");
        }
    }
    else {
        // CEP sem valor, limpa formulário.
        // limpa_formulário_cep();
        alert("Formato de CEP inválido.");
        if (document.getElementById('menu-resultado').display === "none") {
            document.getElementById('logradouro-info').innerHTML = "...";
            document.getElementById('bairro-info').innerHTML = "...";
            document.getElementById('localidade-info').innerHTML = "...";
            document.getElementById('uf-info').innerHTML = "...";
            document.getElementById('ibge-info').innerHTML = "...";
        }
    }
    const found = JSON.parse('{"error" : true}');
    return found;
}

function getAllAddresses() {
    var list = [];
    for (i = 0; i < localStorage.length; i++) {
        var key = localStorage.key(i);
        var item = localStorage.getItem(key);
        if (key.slice(0, 4) === "CEP=") {
            list.push(item);
        }
    }
    return list;
}

function showAddresses(list) {
    if (list.length <= 0) {
        alert("Não há nenhum CEP salvo.");
        return;
    }
    document.getElementById('menu-ceps-salvos').style.display = 'block';
    var div_element = document.getElementById('span-lista-enderecos');
    div_element.innerHTML = "";
    var html_text = "";
    for (let i = 0; i < list.length; i++) {
        html_text = html_text + '<div style="display: block; margin-bottom: 20px;">';
        html_text = html_text + '<text>' + list[i].cep + ', ' + list[i].logradouro + ', ' + list[i].bairro + ', ' + list[i].localidade + ', ' + list[i].uf + ', ' + list[i].ibge + ' </text>'
        html_text = html_text + '<button class="button-13" id="' + list[i].cep + '" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Mais detalhes </button>';
        html_text = html_text + '</div>';
    }
    div_element.innerHTML = html_text;
    // Adicionando eventos aos botões gerados:
    for (let i = 0; i < list.length; i++) {
        var button = document.getElementById(String(list[i].cep));
        button.addEventListener("click", async function () {
            closeDiv('menu-ceps-salvos');
            submitCEP(2, String(list[i].cep));
        });
    }
}

function showUnsortedAddresses() {
    var list = getAllAddresses().map(JSON.parse);
    showAddresses(list);
}

function showSortedAddresses(field) {
    var list = getAllAddresses().map(JSON.parse);
    var href = window.location.origin + "/" + window.location.pathname;
    var server = href.substring(0, href.lastIndexOf('/'));
    server = server + "/sort.php";
    fetch(server, {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8",
        },
        body: `json_list=${JSON.stringify(list)}&field=${field}`,
    })
        .then((response) => response.text())
        .then((res) => (
            // console.log(res)
            // console.log((JSON.parse(res)))
            // showAddresses((JSON.parse(res)).map(JSON.parse))
            showAddresses((JSON.parse(res)))
        ));
}