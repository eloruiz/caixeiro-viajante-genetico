$ErrorActionPreference = 'Stop'
$cultura = [System.Globalization.CultureInfo]::InvariantCulture
$executavel = Join-Path (Split-Path $PSScriptRoot -Parent) 'build/ag.exe'
$pasta = Join-Path (Split-Path $PSScriptRoot -Parent) 'build/integracao'
New-Item -ItemType Directory -Force $pasta | Out-Null
Push-Location $pasta
try {
    foreach ($cenario in @('circular', 'aleatorio')) {
        & $executavel $cenario 12 42 30 20 0.25 0 > "$cenario.txt"
        if ($LASTEXITCODE -ne 0) { throw "Falha ao executar $cenario" }
        $historico = Import-Csv historico.csv
        $pontos = Import-Csv pontos.csv
        if ($historico.Count -ne 21 -or $pontos.Count -ne 12) { throw 'CSV incompleto' }
        $anterior = [double]::PositiveInfinity
        for ($i = 0; $i -lt $historico.Count; ++$i) {
            $linha = $historico[$i]
            $rota = @($linha.rota.Trim().Split(' ') | ForEach-Object { [int]$_ })
            if ([int]$linha.geracao -ne $i -or $rota.Count -ne 12 -or
                ($rota | Sort-Object -Unique).Count -ne 12 -or
                ($rota | Measure-Object -Minimum).Minimum -ne 0 -or
                ($rota | Measure-Object -Maximum).Maximum -ne 11) { throw 'Rota ou geracao invalida no CSV' }
            $distancia = 0.0
            for ($j = 0; $j -lt 12; ++$j) {
                $a = $pontos[$rota[$j]]
                $b = $pontos[$rota[(($j + 1) % 12)]]
                $dx = [double]::Parse($a.x, $cultura) - [double]::Parse($b.x, $cultura)
                $dy = [double]::Parse($a.y, $cultura) - [double]::Parse($b.y, $cultura)
                $distancia += [Math]::Sqrt($dx * $dx + $dy * $dy)
            }
            $registrada = [double]::Parse($linha.melhor_distancia, $cultura)
            if ([Math]::Abs($distancia - $registrada) -gt 1e-8 -or $registrada -gt $anterior) {
                throw 'Distancia exportada incorreta ou elitismo violado'
            }
            $anterior = $registrada
        }
        $primeiro = Get-Content historico.csv -Raw
        & $executavel $cenario 12 42 30 20 0.25 0 > "$cenario.txt"
        if ($LASTEXITCODE -ne 0 -or (Get-Content historico.csv -Raw) -cne $primeiro) { throw 'CSV nao reproduzivel' }
        Copy-Item historico.csv "$cenario-historico.csv"
        Copy-Item pontos.csv "$cenario-pontos.csv"
        Write-Output "PASSOU: CLI e CSV $cenario, 21 geracoes verificadas por calculo independente e reproducao exata"
    }
    $invalidos = @(
        @('invalido'), @('circular','7'), @('circular','12abc'),
        @('circular','12','-1'), @('circular','12','42','1'),
        @('circular','12','42','100','-1'),
        @('circular','12','42','100','10','-0.1'),
        @('circular','12','42','100','10','1.1'),
        @('circular','12','42','100','10','NaN'),
        @('circular','12','42','100','10','0.1','-1'),
        @('circular','12','42','100','10','0.1','0','extra')
    )
    foreach ($argumentos in $invalidos) {
        # Redireciona stderr sem transformar a falha esperada em excecao no Windows PowerShell.
        $preferencia = $ErrorActionPreference
        $ErrorActionPreference = 'Continue'
        $saida = & $executavel @argumentos 2>&1
        $codigo = $LASTEXITCODE
        $ErrorActionPreference = $preferencia
        if ($codigo -ne 1 -or ($saida -join "`n") -notmatch 'Erro:') { throw 'CLI aceitou entrada invalida ou nao explicou erro' }
    }
    Write-Output "PASSOU: CLI rejeitou $($invalidos.Count) entradas invalidas com codigo 1 e mensagem de erro"
} finally { Pop-Location }
