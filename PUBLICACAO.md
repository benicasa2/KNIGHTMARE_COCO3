# Publicação no GitHub

Repositório: [benicasa2/KNIGHTMARE_COCO3](https://github.com/benicasa2/KNIGHTMARE_COCO3).

Versão: [v0.2.1](https://github.com/benicasa2/KNIGHTMARE_COCO3/releases/tag/v0.2.1).

## Anexos da Release

- Jogo compilado para DAC6 e Orchestra-90, disco LEVELS, scripts e manual.
- Fontes, recursos, ferramentas, testes, relatórios e pacotes históricos.
- Manual ilustrado separado em PDF.
- SHA256SUMS.txt para conferir os três arquivos.

## Repetir o procedimento

Com GitHub CLI instalado e autenticado normalmente na conta benicasa2, execute na raiz:

```powershell
& .\tools\publish_github.ps1
```

O script confere a conta, verifica os hashes dos discos e monta os pacotes. Cria o repositório público se necessário, envia branch e tag e publica a Release. Não imprime senhas ou tokens e não substitui Releases existentes. Se a versão já estiver publicada, o procedimento para antes de substituir seus anexos.

Se partir do ZIP de fontes, o script também inicializa o Git local e cria o commit e a tag. Para uma versão futura, atualize VERSION e as notas de versão antes de preparar uma nova tag. Os relatórios de validação precisam corresponder aos discos distribuídos.

A automação .github/workflows/release.yml também permite gerar os anexos e publicar por execução manual no GitHub. O texto desta versão está em docs/release-notes-v0.2.1.md.
