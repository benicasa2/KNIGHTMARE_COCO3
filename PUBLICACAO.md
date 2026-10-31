# Publicação no GitHub

Destino: `benicasa2/KNIGHTMARE_COCO3`. Tag preparada: `v0.2.1`.

**Estado desta entrega: preparado localmente e ainda não publicado.** O ambiente não disponibilizou acesso autenticado utilizável ao GitHub. A consulta pública retornou HTTP 404, que não distingue um repositório inexistente de um privado.

Os anexos estão em `release/`. Com GitHub CLI instalado e autenticado normalmente na conta benicasa2, execute na raiz:

```powershell
& .\tools\publish_github.ps1
```

O script cria o repositório público se necessário, envia branch e tag e publica a Release com jogo, manual, fontes e checksums. Não recebe nem imprime senhas ou tokens e não substitui Releases existentes. A publicação só deve ser considerada concluída após a verificação das URLs exibidas pelo GitHub.

Se partir do ZIP de fontes, o script também inicializa o Git local e cria o commit e a tag. A pasta de trabalho original já tem esse repositório preparado. O arquivo bundle fornecido localmente permite restaurar o histórico Git com `git clone CAMINHO_DO_BUNDLE KNIGHTMARE_COCO3`.

Alternativa: crie o repositório no GitHub, envie main e v0.2.1 e anexe os arquivos de `release/`. O texto está em `docs/release-notes-v0.2.1.md`.

A automação `.github/workflows/release.yml` também permite gerar os anexos e publicar por execução manual no GitHub.
