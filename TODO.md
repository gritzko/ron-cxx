# TODOs

## 1 hour (must be separate commits)

- [x] nicer HasBranch/GetBranch convention
- [ ] branch metadata: names / last name op, now

- [ ] Replica metadata tests (now, origin, sig)
- [ ] only one template instantiation, in libswarmdb.so
- [x] ninja build
- [x] CF list, option
- [x] Store{uuid}, not {path}
- [x] Replica::See()
- [ ] kill merged\_ in MemStore
- [ ] iter key() value() const

## 1 day

- [ ] String vs Codepoint in Cursor/Builder - document
- [x] separate map/,  Mapper<Store> not <Frame>
- [ ]  (MemStore goes to RDT as btree is header-only)
       still actual: sst, memstore/files, closed notation, swarmdb map
- [ ] Builder.AppendXyz may fail (Status? exceptions?)
- [ ] RocksDB frame header stripping
- [x] CLI args are RON, mapper args too - Op, Frame.GetXxx(atom)
- [ ] RDT-level fuzzing -- RGA/CT has a memory bug!
- [ ] use https://github.com/tplgy/cppcodec/

## 1 week

- [ ] txt mapper - full cycle (read, write, utf8, diff)
- [ ] port the connection state machine (tcp, ssh, std)
- [ ] toy revctrl - branch jumping, diffs, 1 workflow (docs, pics)
- [ ] db-level fuzzing
- [ ] toy package mgmt - checkout, deps, build, docs
