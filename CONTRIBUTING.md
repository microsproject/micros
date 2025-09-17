# 📄 Contributing to MicrOS

Thanks for your interest! MicrOS grows with community involvement.

## How to contribute
1. **Fork & branch**
   - Fork `micros` on GitHub
   - Create feature branch: `git checkout -b feature/my-fix`
2. **Code style**
   - C99, `-Wall -Wextra -Werror`
   - 4-space indent, no tabs
   - SPDX header: `// SPDX-License-Identifier: Apache-2.0`
3. **Commit messages**
   - Use short prefix: `kernel: fix timer race`
   - One logical change per commit
4. **Tests**
   - Add unit tests (POSIX port)
   - Samples for new drivers
5. **CI**
   - Ensure GitHub Actions passes
   - Run `pre-commit run --all-files`

## Pull Request process
- Open PR against `main`
- At least 1 review required
- CI must be green
- Squash commits if asked by maintainers

## Reporting Issues
- Use GitHub Issues
- Include board/arch, steps to reproduce, logs if possible
