# Contributing to the project

You can contribute to the project by reporting bugs and submitting pull requests for code changes.

## Reporting bugs

If you have problems using the Self C++ SDK, search for similar issues because someone else might have encountered your problem already. If no one else has previously reported your problem, submit a [bug report](https://github.com/watson-developer-cloud/self-sdk/issues).

## Submitting pull requests

If you want to contribute, follow these steps:

  1. Fork the repository.
  2. Develop and test your code changes.
    * Respect the original code [style guide][style guide].
    * Create minimal diffs - disable on save actions like reformat source code or organize imports. If you think the source code needs to be reformatted, create a separate PR for this change.
    * Check for any unnecessary whitespace by issuing `git diff --check` before you commit.
  3. Ensure your code changes pass your tests.
  4. Commit your changes, following these guidelines: 
    * Use the present tense (for example, `"Add feature"` not `"Added feature"`).
    * Use the imperative mood (for example, `"Move cursor to …"` not `"Moves cursor to …"`).
    * Include the appropriate Emoji from our [Emoji cheatsheet](#emoji-cheatsheet).
  5. Push your changes to your fork and submit a pull request to the master branch.

### Emoji cheatsheet

When you make commits or update the CHANGELOG, please start the commit message or update with one of the following applicable emoji. The emoji should not be used at the start of issue or pull request titles.

* :new: `:new:` when adding new functionality
* :bug: `:bug:` when fixing a bug
* :memo: `:memo:` when writing documentation
* :art: `:art:` when improving the format/structure of the code
* :fire: `:fire:` when removing code or files
* :racehorse: `:racehorse:` when improving performance
* :white_check_mark: `:white_check_mark:` when adding tests
* :green_heart: `:green_heart:` when fixing the CI build
* :lock: `:lock:` when dealing with security
* :crystal_ball: `:crystal_ball:` when experimenting
* :unamused: `:unamused:` when doing chore work (updating dependencies, etc…)
* :boom: `:boom:` when changing in a non-backwards-compatible way current functionality
* :shipit: `:shipit:` when creating a new release


## Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
   have the right to submit it under the open source license
   indicated in the file; or

(b) The contribution is based upon previous work that, to the best
   of my knowledge, is covered under an appropriate open source
   license and I have the right under that license to submit that
   work with modifications, whether created in whole or in part
   by me, under the same open source license (unless I am
   permitted to submit under a different license), as indicated
   in the file; or

(c) The contribution was provided directly to me by some other
   person who certified (a), (b) or (c) and I have not modified
   it.

(d) I understand and agree that this project and the contribution
   are public and that a record of the contribution (including all
   personal information I submit with it, including my sign-off) is
   maintained indefinitely and may be redistributed consistent with
   this project or the open source license(s) involved.

## Additional resources
  * [General GitHub documentation](https://help.github.com/)
  * [GitHub pull request documentation](https://help.github.com/send-pull-requests/)

[dw]: https://developer.ibm.com/answers/questions/ask/?topics=watson
[stackoverflow]: http://stackoverflow.com/questions/ask?tags=ibm-watson
[style guide]: https://google.github.io/styleguide/cppguide.html
