$ErrorActionPreference = 'Stop'
$Repo = 'benicasa2/KNIGHTMARE_COCO3'
$Root = Split-Path -Parent $PSScriptRoot
Set-Location -LiteralPath $Root
foreach ($Tool in @('gh','git','python')) {
    if (-not (Get-Command $Tool -ErrorAction SilentlyContinue)) {
        throw "Required command missing: $Tool. Install it and authenticate GitHub CLI normally before publishing."
    }
}
$Login = & gh api user --jq '.login'
if ($LASTEXITCODE -ne 0) { throw 'Authenticate GitHub CLI with gh auth login, then retry.' }
if ($Login.Trim() -ne 'benicasa2') { throw 'The authenticated account must be benicasa2.' }
$Version = (Get-Content -LiteralPath VERSION -Raw).Trim()
$Tag = "v$Version"
& python tools/package_release.py
if ($LASTEXITCODE -ne 0) { throw 'Release packaging failed.' }
if (-not (Test-Path -LiteralPath '.git')) {
    & git init -b main
    if ($LASTEXITCODE -ne 0) { throw 'Local repository initialization failed.' }
    & git add .
    if ($LASTEXITCODE -ne 0) { throw 'Local staging failed.' }
    & git -c user.name=benicasa2 -c user.email=benicasa2@users.noreply.github.com commit -m 'Prepare Knightmare CoCo3 v0.2.1 with illustrated manual'
    if ($LASTEXITCODE -ne 0) { throw 'Local commit failed.' }
    & git -c user.name=benicasa2 -c user.email=benicasa2@users.noreply.github.com tag -a $Tag -m 'Two stages, reduced flicker and illustrated manual'
    if ($LASTEXITCODE -ne 0) { throw 'Local tag creation failed.' }
}
& gh repo view $Repo --json nameWithOwner 2>$null
if ($LASTEXITCODE -ne 0) {
    & gh repo create $Repo --public --description 'Knightmare for CoCo 3 128 KiB in 6809 Assembly with illustrated manual'
    if ($LASTEXITCODE -ne 0) { throw 'Repository creation failed.' }
}
$Remote = & git remote get-url origin 2>$null
if ($LASTEXITCODE -ne 0) {
    & git remote add origin "https://github.com/$Repo.git"
} elseif ($Remote.Trim() -notin @("https://github.com/$Repo.git","git@github.com:$Repo.git")) {
    throw 'Existing origin does not point to the requested repository.'
}
& git push -u origin main
if ($LASTEXITCODE -ne 0) { throw 'Branch push failed. No forced push will be attempted.' }
& git show-ref --verify --quiet "refs/tags/$Tag"
if ($LASTEXITCODE -ne 0) { throw 'Expected local release tag is missing.' }
& git push origin "refs/tags/$Tag"
if ($LASTEXITCODE -ne 0) { throw 'Tag push failed.' }
& gh release view $Tag --repo $Repo --json url 2>$null
if ($LASTEXITCODE -eq 0) { throw 'Release already exists; existing assets will not be replaced.' }
$Assets = @(
    "release/KNIGHTMARE_COCO3_v${Version}_Jogo.zip",
    "release/KNIGHTMARE_COCO3_v${Version}_Fontes.zip",
    "release/KNIGHTMARE_COCO3_Manual_v${Version}.pdf",
    'release/SHA256SUMS.txt'
)
& gh release create $Tag @Assets --repo $Repo --verify-tag --title "$Tag Duas fases e menos cintilacao" --notes-file "docs/release-notes-$Tag.md"
if ($LASTEXITCODE -ne 0) { throw 'GitHub Release creation failed.' }
& gh release view $Tag --repo $Repo --json url,tagName,isDraft,isPrerelease
if ($LASTEXITCODE -ne 0) { throw 'Release was sent but remote verification failed.' }
