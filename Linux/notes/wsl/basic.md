# wsl command

## check wsl version:
1. `wsl --list` `wsl -l`
2. `wsl --list --online` `wsl -l -o`
3. check status: `wsl -l -v`

## start specific version

1. start default "*" version: `wsl`
2. start specific version: `wsl -d <release-name>`

## change default version

`wsl --set-default <release-name>`
`wsl -s <release-name>`
