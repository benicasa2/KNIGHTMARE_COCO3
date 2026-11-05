# Publicação no GitHub

Repositório: [benicasa2/KNIGHTMARE_COCO3](https://github.com/benicasa2/KNIGHTMARE_COCO3).

Versão: [v0.3.0](https://github.com/benicasa2/KNIGHTMARE_COCO3/releases/tag/v0.3.0).

## Anexos da Release

- Jogo compilado para DAC6, Orchestra-90 e SSC original, disco LEVELS, scripts e manual.
- Fontes, recursos, ferramentas, testes, relatórios e pacotes históricos.
- Manual ilustrado v0.2.1 separado em PDF e suplemento SSC.md da v0.3.0.
- SHA256SUMS.txt para conferir os quatro arquivos.

## Repetir o procedimento

Com GitHub CLI instalado e autenticado normalmente na conta benicasa2, execute na raiz:

```powershell
& .\tools\publish_github.ps1
```

O script confere a conta, verifica os hashes dos discos e monta os pacotes. Cria o repositório público se necessário, envia branch e tag e publica a Release. Não imprime senhas ou tokens e não substitui Releases existentes. Se a versão já estiver publicada, o procedimento para antes de substituir seus anexos.

Se partir do ZIP de fontes, o script também inicializa o Git local e cria o commit e a tag. Para uma versão futura, atualize VERSION e as notas de versão antes de preparar uma nova tag. Os relatórios de validação precisam corresponder aos discos distribuídos.

A automação .github/workflows/release.yml também permite gerar os anexos e publicar por execução manual no GitHub. O texto desta versão está em docs/release-notes-v0.3.0.md.
