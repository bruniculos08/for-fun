<?php
    $json_list = $_POST['json_list'];
    $field = $_POST['field'];
    // $json_list = '[{"cep":"89222-430","logradouro":"Rua Ituporanga","complemento":"","unidade":"","bairro":"Bom Retiro","localidade":"Joinville","uf":"SC","estado":"Santa Catarina","regiao":"Sul","ibge":"4209102","gia":"","ddd":"47","siafi":"8179"},{"cep":"01153-000","logradouro":"Rua Vitorino Carmilo","complemento":"","unidade":"","bairro":"Barra Funda","localidade":"São Paulo","uf":"SP","estado":"São Paulo","regiao":"Sudeste","ibge":"3550308","gia":"1004","ddd":"11","siafi":"7107"}]';
    // $field = "bairro";
    
    $json_array = json_decode($json_list, true);
    
    $f = function($a, $b) use ($field) { return strcasecmp($a[$field], $b[$field]); };

    usort($json_array, $f);
    // print_r($json_array);
    
    echo (json_encode($json_array));
?>
