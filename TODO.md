# TODOs

## 1 hour (must be separate commits)

- [ ] Replica metadata tests (now, origin, sig)
- [ ] kill merged\_ in MemStore
- [ ] only one template instantiation, in libswarmdb.so
- [x] ninja build
- [ ] CF list, option
- [ ] Store{uuid}, not {path}
- [ ] Replica::See()
- [ ] iter key() value() const

## 1 day

- [x] separate map/,  Mapper<Store> not <Frame>
- [ ]  (MemStore goes to RDT as btree is header-only)
- [ ] Builder.AppendXyz may fail (Status? exceptions?)
- [ ] RocksDB frame header stripping
- [ ] CLI args are RON, mapper args too - Op, Frame.GetXxx(atom)
- [ ] RDT-level fuzzing

## 1 week

- [ ] txt mapper - full cycle (read, write, utf8, diff)
- [ ] port the connection state machine (tcp, ssh, std)
- [ ] toy revctrl - branch jumping, diffs, 1 workflow (docs, pics)
- [ ] db-level fuzzing
- [ ] toy package mgmt - checkout, deps, build, docs
