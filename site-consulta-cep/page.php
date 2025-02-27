<!DOCTYPE html>

<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
<link rel="stylesheet" href="button.css">
<link rel="stylesheet" href="menu.css">
<link rel="icon" type="image/x-icon" href="https://images.vexels.com/media/users/3/144131/isolated/preview/29576a7e0442960346703d3ecd6bac04-icone-de-doodle-de-imagem.png">
<script type="text/javascript" src="system.js"></script>

<head>
    <html lang="pt-br">
    <title> CEP API </title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: 'Courier New', Courier, monospace;
            background-image: url('https://images.pexels.com/photos/1389339/pexels-photo-1389339.jpeg?cs=srgb&dl=pexels-wildlittlethingsphoto-1389339.jpg&fm=jpg');
            background-position: center;
            background-size: cover;
            /* Adicionado para forçar a tag article a ocupar o meio da página inteira,...
            ... isto é, forçar o footer abaixo */
            display: flex;
            flex-direction: column;
            min-height: 100vh;
            margin: 0;
        }

        input[type=text],
        select {
            /* width: 100%; */
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }
    </style>

    </html>
</head>

<body>
    <div class="w3-top">
        <div class="w3-row w3-padding w3-black">
            <div class="w3-col s3">
                <a onclick="document.getElementById('menu-pesquisa').style.display='block'"
                    class="w3-button w3-block w3-black"> Pesquisar CEP </a>
            </div>
            <div class="w3-col s3">
                <!-- <a onclick="document.getElementById('menu-ceps-salvos').style.display='block'"
                    class="w3-button w3-block w3-black"> Mostrar CEP's salvos </a> -->
                <a onclick="showUnsortedAddresses()" class="w3-button w3-block w3-black"> Mostrar CEP's salvos </a>
            </div>
        </div>
    </div>

    <!-- Menu de Pesquisa -->
    <!-- Pop-up (parte de fora que bloqueia o resto da página)-->
    <div id="menu-pesquisa" class="w3-modal descend-animation">
        <!-- Pop-up -->
        <div style="width: 500px; top: 30%; transition: height 10s;" class="w3-modal-content descend-animation expand">
            <!-- class="w3-modal-content w3-animate-zoom descend-animation expand"> -->
            <!-- Faixa preta de título -->
            <div class="w3-container w3-white w3-display-container">
                <!-- Botão para fechar -->
                <!-- <span onclick="closeMenu()" class="w3-button w3-display-topright w3-small">x</span> -->
                <span onclick="closeDiv('menu-pesquisa');" class="w3-button w3-display-topright w3-small">x</span>
                <!-- <h1>Pesquisar CEP</h1> -->
            </div>
            <div class="w3-container" style="text-align: center;">
                <script type="text/javascript" src="system.js"></script>
                <form id="pesquisar-cep" method="dialog" onsubmit="submitCEP(0);">
                    <!-- <form id="pesquisar-cep"> -->
                    <br>
                    <text id="cep-digitado"> </text>
                    <label for="texto-cep"> CEP: </label>
                    <input type="text" id="texto-cep" name="texto-cep">
                    <br><br>
                    <!-- <button type="reset"> Limpar </button> -->
                    <!-- <button id="ButtonPesquisar" type="button"> Pesquisar </button> -->
                    <button class="button-13" id="ButtonPesquisar" type="button">Pesquisar </button>
                    <br><br>
                    <script>
                        var form = document.getElementById("pesquisar-cep");
                        document.getElementById("ButtonPesquisar").addEventListener("click", function () {
                            // form.submit();
                            submitCEP(0);
                        });
                    </script>
                </form>
            </div>
        </div>
    </div>

    <!-- Menu de Resultado de Pesquisa -->
    <!-- Pop-up (parte de fora que bloqueia o resto da página)-->
    <div id="menu-resultado" class="w3-modal descend-animation">
        <!-- Pop-up -->
        <div style="width: 500px; top: 5%; transition: height 10s;" class="w3-modal-content descend-animation expand">
            <!-- class="w3-modal-content w3-animate-zoom descend-animation expand"> -->
            <!-- Faixa preta de título -->
            <div class="w3-container w3-white w3-display-container">
                <!-- Botão para fechar -->
                <span onclick="closeDiv('menu-resultado');" class="w3-button w3-display-topright w3-small">x</span>
            </div>
            <div class="w3-container" style="text-align: center;">
                <script type="text/javascript" src="system.js"></script>
                <form id="dados-resultado" method="dialog" onsubmit="submitCEP(1);">
                    <br>
                    <text id="cep-digitado"> </text>
                    <label for="texto-cep"> CEP: </label>
                    <input type="text" id="texto-cep2" name="texto-cep">
                    <br><br>

                    <div style="text-align: left; margin-left: 10%;">

                        <br>
                        <text id="cep-pesquisado"> </text>
                        <label for="cep-pesquisado" style="font-weight: bold;"> CEP pesquisado: </label>
                        <text id="cep-pesquisado-info"> ... </text>
                        <br><br>

                        <br>
                        <text id="logradouro"> </text>
                        <label for="logradouro" style="font-weight: bold;"> Logradouro: </label>
                        <text id="logradouro-info"> ... </text>
                        <br><br>

                        <br>
                        <text id="bairro"> </text>
                        <label for="bairro" style="font-weight: bold;"> Bairro: </label>
                        <text id="bairro-info"> ... </text>
                        <br><br>

                        <br>
                        <text id="localidade"> </text>
                        <label for="localidade" style="font-weight: bold;"> Localidade: </label>
                        <text id="localidade-info"> ... </text>
                        <br><br>

                        <br>
                        <text id="uf"> </text>
                        <label for="uf" style="font-weight: bold;"> U.F.: </label>
                        <text id="uf-info"> ... </text>
                        <br><br>

                        <br>
                        <text id="ibge"> </text>
                        <label for="ibge" style="font-weight: bold;"> IBGE: </label>
                        <text id="ibge-info"> ... </text>
                        <br><br>

                    </div>

                    <button class="button-13" id="ButtonPesquisar2" type="button">Pesquisar </button>
                    <br><br>
                    <script>
                        var form = document.getElementById("pesquisar-cep");
                        document.getElementById("ButtonPesquisar2").addEventListener("click", async function () {
                            // form.submit();
                            // closeDiv('menu-resultado');
                            submitCEP(1);
                        });
                    </script>
                </form>
            </div>
        </div>
    </div>

    <!-- Menu de CEP's Salvos -->
    <!-- Pop-up (parte de fora que bloqueia o resto da página)-->
    <div id="menu-ceps-salvos" class="w3-modal descend-animation">
        <!-- Pop-up -->
        <div style="width: auto; top: 30%; transition: height 10s; margin: 70px" class="w3-modal-content descend-animation expand">
            <!-- class="w3-modal-content w3-animate-zoom descend-animation expand"> -->
            <!-- Faixa preta de título -->
            <div class="w3-container w3-white w3-display-container">
                <!-- Botão para fechar -->
                <!-- <span onclick="closeMenu()" class="w3-button w3-display-topright w3-small">x</span> -->
                <span onclick="closeDiv('menu-ceps-salvos');" class="w3-button w3-display-topright w3-small">x</span>
                <!-- <h1>Pesquisar CEP</h1> -->
            </div>

            <div id="lista-enderecos" class="w3-container"
                style="max-height: 200px; overflow-y: auto; overflow-x: auto; text-align: left; margin-top: 40px; margin-bottom: 200px;
                margin-left: 40px">

                <script type="text/javascript" src="system.js"></script>

                <div style="display: block; margin-bottom: 20px;">
                    <form method="dialog">
                        <label for="sort-selection"> Ordenação: </label>
                        <select name="sort-selection" id="sort-selection">
                            <option value=""> ... </option>
                            <option value="cep"> CEP </option>
                            <option value="logradouro"> Logradouro </option>
                            <option value="bairro"> Bairro </option>
                            <option value="localidade"> Localidade </option>
                            <option value="uf"> U.F. </option>
                            <option value="ibge"> IBGE </option>
                        </select>
                        <button class="button-13" id="sort-button" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Ordenar </button>
                    </form>
                    <script>
                        var sort_button = document.getElementById("sort-button");
                        var selection = document.getElementById("sort-selection");
                        sort_button.addEventListener("click", function () {
                            if(selection.value === "")
                                showUnsortedAddresses();
                            else
                                showSortedAddresses(selection.value);
                        });
                    </script>
                </div>
                
                <span id="span-lista-enderecos">
                    
                </span>

                <!-- <div style="display: block; margin-bottom: 20px;">
                    <text> dsadasda </text> <button class="button-13" id="dsadasda" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Mais detalhes </button>
                </div>
                
                <div style="display: block; margin-bottom: 20px;">
                    <text> dsadasda </text> <button class="button-13" id="dsadasda" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Mais detalhes </button>
                </div>

                <div style="display: block; margin-bottom: 20px;">
                    <text> dsadasda </text> <button class="button-13" id="dsadasda" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Mais detalhes </button>
                </div>

                <div style="display: block; margin-bottom: 20px;">
                    <text> dsadasda </text> <button class="button-13" id="dsadasda" type="button" style="margin-left: 2px; margin-right: 2px; width: 120px;"> Mais detalhes </button>
                </div> -->
                <!-- <form id="pesquisar-cep" onsubmit="submitCEP(0)">
                    <br>
                    <text id="cep-digitado"> </text>
                    <label for="texto-cep"> CEP: </label>
                    <input type="text" id="texto-cep" name="texto-cep">
                    <br><br>
                    <button class="button-13" id="ButtonPesquisar" type="button">Pesquisar </button>
                    <br><br>
                    <script>
                        var form = document.getElementById("pesquisar-cep");
                        document.getElementById("ButtonPesquisar").addEventListener("click", function () {
                            submitCEP(0);
                        });
                    </script>
                </form> -->
            </div>
        </div>
    </div>

</body>