# IdeaTek

IdeaTek is a set of contracts for game engines on Bridge. Each concern is a part with its own contract.

## Install

Python 3.11 or a later version.

## Usage

| Command | Result |
| --- | --- |
| `python project.py fetch` | Downloads what the build needs. |
| `python project.py build` | Builds the project. |
| `python project.py pack` | Packs the build for a release. |
| `python project.py test` | Runs the tests. |
| `python project.py test --break` | Runs the tests with their claims made false. Each test must fail. |
| `python project.py test --branches` | Runs the tests on a build with probes. Each branch of the code must run. |
| `python project.py test --leaks` | Runs the tests on a build with counts. Each test must give back what the code takes. |
| `python project.py check` | Checks the project. |
| `python project.py clean` | Removes what the build made. Keeps what `fetch` downloaded. |
| `python project.py distclean` | Removes the folder `build`, also what `fetch` downloaded. |

### The contracts

Each part has a header in `include` and a document in `docs/extern`, which gives its types.

- Each module in C that writes or reads a type of a part includes the header of the part.
- Each macro of the text gives the braces of its type and the text between them.
- A module in another language does not include a header. It states the descriptor of each type with the bytes of the document.
- Each module that uses a type declares it in its `init`. A module in C gives the C type and the descriptor of the header.
- A module declares a type after the types of its fields.
- Bridge compares the descriptors byte for byte. It also refuses a second module that states another kind, storage layout or `on_destroy`.
- Bridge does not compare `on_unload`, so each module states its own.
- No module has the namespace `ideatek`. Thus each module needs a grant for each type that it declares, in `read` or `write` of the deployment.
- Two modules that conflict in one phase need a pair of the deployment.

The documents of `bridge.h` and `bridge_abi.h` of Bridge tell the pairs and what each grant allows.

## Contributing

| Document | Content |
| --- | --- |
| `docs/contributing/needs.md` | The needs that each decision meets. |
| `docs/contributing/decisions.md` | The steps of a decision. |
| `docs/contributing/layers.md` | The layers of an engine, and the layers of the project. |
| `docs/contributing/coding_style.md` | How to write the code. |
| `docs/contributing/documentation_style.md` | How to write the documents. |
| `docs/contributing/tests.md` | How to write the tests. |
| `docs/contributing/commits.md` | How to write a commit. |
| `docs/contributing/pull_request.md` | How to write a pull request. |
| `docs/contributing/agreement.md` | The agreement `cla.md`, which each contributor signs. |

## License

The GNU Affero General Public License, version 3 only. The file `license` holds the text.
Each source of third-party code has its notice in `third_party/<source>/license`.
