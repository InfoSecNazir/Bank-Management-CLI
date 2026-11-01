# Bank-Management-CLI

[English](README.md) · [العربية](README_AR.md) · [File index](docs/INDEX.md)

Educational C++ console banking application for client records, users, balances, and transactions.

## Repository contents

- [main.cpp](main.cpp)
- [MyBankProject.cpp](MyBankProject.cpp)

## Setup and use

Two independent source versions are included, each with its own `main`. Build one at a time, for example in a Visual Studio Developer Command Prompt:

```bat
cl /EHsc /std:c++17 main.cpp /Fe:bank.exe
bank.exe
```

The source reads `Clients.txt` and `Users.txt` in the working directory. Use fictional fixtures matching the selected version's record format. Do not link both source files into one executable.

The console code uses Windows commands such as `cls` or `pause`; Windows is the intended environment. Build commands below describe the source entry point and have not been compiled as part of this documentation update.

## Notes

This repository demonstrates console programming and file persistence. It has not been validated for real financial use.

[Previous documentation and additional details](README_DETAILS.md)

## License

The repository currently has no root license file. This documentation does not assign a new license.

## Contributing and navigation

[CONTRIBUTING.md](CONTRIBUTING.md) · [Documentation index](docs/INDEX.md) · [Public project index](https://github.com/InfoSecNazir/InfoSecNazir/blob/main/PROJECTS.md)

## Verification scope

Repository organization checked file paths, documentation links, and available dependency manifests. This update did not run or deploy the application.
