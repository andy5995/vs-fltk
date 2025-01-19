Code generation is extremely important in this project, to ensure:

- Multiple targets can be automatically constructed from the same specifications
- They will be aligned, simplifying versioning

Code generation tasks are performed by typescript code. They are based on information defined in `/schemas` and run off scripts in `/scripts/codegen`.  
This choice of technologies means that installing the [bun js runtime](https://bun.sh/) is needed to build the project (and only that).

## Targets for generation

### Database

### Widgets

### Enumerations
