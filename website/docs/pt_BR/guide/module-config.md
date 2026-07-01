# Configuração de Módulo

O SxKernelSU fornece um sistema de configuração integrado que permite que os módulos armazenem configurações de chave-valor persistentes ou temporárias. As configurações são armazenadas em formato binário em `/data/adb/sks/module_configs/<module_id>/` com as seguintes características:

## Tipos de Configuração

- **Configuração Persistente** (`persist.config`): Sobrevive às reinicializações até ser explicitamente excluída ou o módulo ser desinstalado
- **Configuração Temporária** (`tmp.config`): Automaticamente limpa durante o estágio post-fs-data em cada inicialização

Ao ler configurações, os valores temporários têm prioridade sobre os valores persistentes para a mesma chave.

## Usando Configuração em Scripts de Módulo

Todos os scripts de módulo (`post-fs-data.sh`, `service.sh`, `boot-completed.sh`, etc.) são executados com a variável de ambiente `SKS_MODULE` definida como o ID do módulo. Você pode usar os comandos `sksud module config` para gerenciar a configuração do seu módulo:

```bash
# Obter um valor de configuração
value=$(sksud module config get my_setting)

# Definir um valor de configuração persistente
sksud module config set my_setting "some value"

# Definir um valor de configuração temporário (limpo após a reinicialização)
sksud module config set --temp runtime_state "active"

# Definir valor a partir de stdin (útil para texto multilinhas ou dados complexos)
sksud module config set my_key <<EOF
texto multilinhas
valor
EOF

# Ou transmitir de um comando
echo "value" | sksud module config set my_key

# Sinalizador stdin explícito
cat file.json | sksud module config set json_data --stdin

# Listar todas as entradas de configuração (mesclando persistentes e temporárias)
sksud module config list

# Excluir uma entrada de configuração
sksud module config delete my_setting

# Excluir uma entrada de configuração temporária
sksud module config delete --temp runtime_state

# Limpar todas as configurações persistentes
sksud module config clear

# Limpar todas as configurações temporárias
sksud module config clear --temp
```

## Limites de Validação

O sistema de configuração impõe os seguintes limites:

- **Comprimento máximo da chave**: 256 bytes
- **Comprimento máximo do valor**: 1MB (1048576 bytes)
- **Número máximo de entradas de configuração**: 32 por módulo
- **Formato de chave**: Deve corresponder a `^[a-zA-Z][a-zA-Z0-9._-]+$` (como ID do módulo)
  - Deve começar com uma letra
  - Pode conter letras, números, pontos, sublinhados ou hífens
  - Comprimento mínimo: 2 caracteres
- **Formato de valor**: Sem restrições - pode conter qualquer caractere UTF-8, incluindo quebras de linha e caracteres de controle
  - Armazenado em formato binário com prefixo de comprimento para manuseio seguro de todos os dados

## Ciclo de Vida

- **Na inicialização**: Todas as configurações temporárias são limpas durante o estágio post-fs-data
- **Na desinstalação do módulo**: Todas as configurações (persistentes e temporárias) são automaticamente removidas
- As configurações são armazenadas em formato binário com número mágico `0x4b53554d` ("KSUM") e validação de versão

## Casos de Uso

O sistema de configuração é ideal para:

- **Preferências do usuário**: Armazenar configurações de módulo que os usuários configuram por meio de WebUI ou scripts de ação
- **Sinalizadores de recursos**: Ativar/desativar recursos do módulo sem reinstalar
- **Estado de execução**: Rastrear estado temporário que deve ser redefinido na reinicialização (use configuração temporária)
- **Configurações de instalação**: Lembrar escolhas feitas durante a instalação do módulo
- **Dados complexos**: Armazenar JSON, texto multilinha, dados codificados em Base64 ou qualquer conteúdo estruturado (até 1MB)

::: tip MELHORES PRÁTICAS
- Use configurações persistentes para preferências do usuário que devem sobreviver às reinicializações
- Use configurações temporárias para estado de execução ou sinalizadores de recursos que devem ser redefinidos na inicialização
- Valide os valores de configuração em seus scripts antes de usá-los
- Use o comando `sksud module config list` para depurar problemas de configuração
:::

## Recursos Avançados

O sistema de configuração de módulos fornece chaves de configuração especiais para casos de uso avançados:

### Substituindo a Descrição do Módulo {#overriding-module-description}

Você pode substituir dinamicamente o campo `description` do `module.prop` definindo a chave de configuração `override.description`:

```bash
# Substituir a descrição do módulo
sksud module config set override.description "Descrição personalizada exibida no gerenciador"
```

Ao recuperar a lista de módulos, se a configuração `override.description` existir, ela substituirá a descrição original do `module.prop`. Isso é útil para:
- Exibir informações dinâmicas de status na descrição do módulo
- Mostrar detalhes de configuração em tempo de execução aos usuários
- Atualizar a descrição com base no estado do módulo sem reinstalar

### Declarando Recursos Gerenciados

Os módulos podem declarar quais recursos do SxKernelSU eles gerenciam usando o padrão de configuração `manage.<feature>`. Os recursos suportados correspondem ao enum interno `FeatureId` do SxKernelSU:

**Recursos Suportados:**
- `su_compat` - Modo de compatibilidade SU
- `kernel_umount` - Desmontagem automática do kernel

```bash
# Declarar que este módulo gerencia a compatibilidade SU e a habilita
sksud module config set manage.su_compat true

# Declarar que este módulo gerencia a desmontagem do kernel e a desabilita
sksud module config set manage.kernel_umount false

# Remover gerenciamento de recurso (o módulo não controla mais este recurso)
sksud module config delete manage.su_compat
```

**Como funciona:**
- A presença de uma chave `manage.<feature>` indica que o módulo está gerenciando esse recurso
- O valor indica o estado desejado: `true`/`1` para habilitado, `false`/`0` (ou qualquer outro valor) para desabilitado
- Para parar de gerenciar um recurso, exclua completamente a chave de configuração

Os recursos gerenciados são expostos através da API de lista de módulos como um campo `managedFeatures` (string separada por vírgulas). Isso permite:
- O gerenciador do SxKernelSU detectar quais módulos gerenciam quais recursos do SxKernelSU
- Prevenção de conflitos quando vários módulos tentam gerenciar o mesmo recurso
- Melhor coordenação entre módulos e funcionalidade central do SxKernelSU

::: warning APENAS RECURSOS SUPORTADOS
Use apenas os nomes de recursos predefinidos listados acima (`su_compat`, `kernel_umount`). Eles correspondem aos recursos internos reais do SxKernelSU. Usar outros nomes de recursos não causará erros, mas não terá nenhum propósito funcional.
:::
