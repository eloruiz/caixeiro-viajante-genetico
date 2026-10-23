param([string]$Compilador = 'g++')
$ErrorActionPreference = 'Stop'
Push-Location (Split-Path $PSScriptRoot -Parent)
try {
    New-Item -ItemType Directory -Force build | Out-Null
    $opcoes = @('-std=c++17', '-Wall', '-Wextra', '-Wpedantic', '-Werror')
    foreach ($nome in @('problema', 'cruzamento', 'evolucao', 'requisitos')) {
        $fontes = @("src/$nome.cpp")
        if ($nome -eq 'evolucao') { $fontes += @('src/problema.cpp', 'src/cruzamento.cpp') }
        if ($nome -eq 'requisitos') { $fontes = @('src/problema.cpp', 'src/cruzamento.cpp', 'src/evolucao.cpp') }
        & $Compilador @opcoes @fontes "testes/teste_$nome.cpp" -o "build/teste_$nome.exe"
        if ($LASTEXITCODE -ne 0) { throw "Falha ao compilar $nome" }
        & "./build/teste_$nome.exe"
        if ($LASTEXITCODE -ne 0) { throw "Falha nos testes de $nome" }
    }
    & $Compilador @opcoes src/problema.cpp src/cruzamento.cpp src/evolucao.cpp src/main.cpp -o build/ag.exe
    if ($LASTEXITCODE -ne 0) { throw 'Falha ao compilar o programa' }
    & "$PSScriptRoot/teste_integracao.ps1"
} finally { Pop-Location }
