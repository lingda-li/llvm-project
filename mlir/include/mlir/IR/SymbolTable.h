//===- SymbolTable.h - MLIR Symbol Table Class ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_IR_SYMBOLTABLE_H
#define MLIR_IR_SYMBOLTABLE_H

#include "mlir/IR/Attributes.h"
#include "mlir/IR/OpDefinition.h"
#include "llvm/ADT/StringMap.h"

namespace mlir {
class Identifier;
class Operation;

/// This class allows for representing and managing the symbol table used by
/// operations with the 'SymbolTable' trait. Inserting into and erasing from
/// this SymbolTable will also insert and erase from the Operation given to it
/// at construction.
class SymbolTable {
public:
  /// Build a symbol table with the symbols within the given operation.
  SymbolTable(Operation *symbolTableOp);

  /// Look up a symbol with the specified name, returning null if no such
  /// name exists. Names never include the @ on them.
  Operation *lookup(StringRef name) const;
  template <typename T> T lookup(StringRef name) const {
    return dyn_cast_or_null<T>(lookup(name));
  }

  /// Erase the given symbol from the table.
  void erase(Operation *symbol);

  /// Insert a new symbol into the table, and rename it as necessary to avoid
  /// collisions. Also insert at the specified location in the body of the
  /// associated operation.
  void insert(Operation *symbol, Block::iterator insertPt = {});

  /// Return the name of the attribute used for symbol names.
  static StringRef getSymbolAttrName() { return "sym_name"; }

  /// Returns the associated operation.
  Operation *getOp() const { return symbolTableOp; }

  /// Return the name of the attribute used for symbol visibility.
  static StringRef getVisibilityAttrName() { return "sym_visibility"; }

  //===--------------------------------------------------------------------===//
  // Symbol Utilities
  //===--------------------------------------------------------------------===//

  /// An enumeration detailing the different visibility types that a symbol may
  /// have.
  enum class Visibility {
    /// The symbol is public and may be referenced anywhere internal or external
    /// to the visible references in the IR.
    Public,

    /// The symbol is private and may only be referenced by SymbolRefAttrs local
    /// to the operations within the current symbol table.
    Private,

    /// The symbol is visible to the current IR, which may include operations in
    /// symbol tables above the one that owns the current symbol. `Nested`
    /// visibility allows for referencing a symbol outside of its current symbol
    /// table, while retaining the ability to observe all uses.
    Nested,
  };

  /// Returns the name of the given symbol operation.
  static StringRef getSymbolName(Operation *symbol);
  /// Sets the name of the given symbol operation.
  static void setSymbolName(Operation *symbol, StringRef name);

  /// Returns the visibility of the given symbol operation.
  static Visibility getSymbolVisibility(Operation *symbol);
  /// Sets the visibility of the given symbol operation.
  static void setSymbolVisibility(Operation *symbol, Visibility vis);

  /// Returns the nearest symbol table from a given operation `from`. Returns
  /// nullptr if no valid parent symbol table could be found.
  static Operation *getNearestSymbolTable(Operation *from);

  /// Walks all symbol table operations nested within, and including, `op`. For
  /// each symbol table operation, the provided callback is invoked with the op
  /// and a boolean signifying if the symbols within that symbol table can be
  /// treated as if all uses within the IR are visible to the caller.
  /// `allSymUsesVisible` identifies whether all of the symbol uses of symbols
  /// within `op` are visible.
  static void walkSymbolTables(Operation *op, bool allSymUsesVisible,
                               function_ref<void(Operation *, bool)> callback);

  /// Returns the operation registered with the given symbol name with the
  /// regions of 'symbolTableOp'. 'symbolTableOp' is required to be an operation
  /// with the 'OpTrait::SymbolTable' trait.
  static Operation *lookupSymbolIn(Operation *op, StringRef symbol);
  static Operation *lookupSymbolIn(Operation *op, SymbolRefAttr symbol);
  /// A variant of 'lookupSymbolIn' that returns all of the symbols referenced
  /// by a given SymbolRefAttr. Returns failure if any of the nested references
  /// could not be resolved.
  static LogicalResult lookupSymbolIn(Operation *op, SymbolRefAttr symbol,
                                      SmallVectorImpl<Operation *> &symbols);

  /// Returns the operation registered with the given symbol name within the
  /// closest parent operation of, or including, 'from' with the
  /// 'OpTrait::SymbolTable' trait. Returns nullptr if no valid symbol was
  /// found.
  static Operation *lookupNearestSymbolFrom(Operation *from, StringRef symbol);
  static Operation *lookupNearestSymbolFrom(Operation *from,
                                            SymbolRefAttr symbol);
  template <typename T>
  static T lookupNearestSymbolFrom(Operation *from, StringRef symbol) {
    return dyn_cast_or_null<T>(lookupNearestSymbolFrom(from, symbol));
  }
  template <typename T>
  static T lookupNearestSymbolFrom(Operation *from, SymbolRefAttr symbol) {
    return dyn_cast_or_null<T>(lookupNearestSymbolFrom(from, symbol));
  }

  /// This class represents a specific symbol use.
  class SymbolUse {
  public:
    SymbolUse(Operation *op, SymbolRefAttr symbolRef)
        : owner(op), symbolRef(symbolRef) {}

    /// Return the operation user of this symbol reference.
    Operation *getUser() const { return owner; }

    /// Return the symbol reference that this use represents.
    SymbolRefAttr getSymbolRef() const { return symbolRef; }

  private:
    /// The operation that this access is held by.
    Operation *owner;

    /// The symbol reference that this use represents.
    SymbolRefAttr symbolRef;
  };

  /// This class implements a range of SymbolRef uses.
  class UseRange {
  public:
    UseRange(std::vector<SymbolUse> &&uses) : uses(std::move(uses)) {}

    using iterator = std::vector<SymbolUse>::const_iterator;
    iterator begin() const { return uses.begin(); }
    iterator end() const { return uses.end(); }

  private:
    std::vector<SymbolUse> uses;
  };

  /// Get an iterator range for all of the uses, for any symbol, that are nested
  /// within the given operation 'from'. This does not traverse into any nested
  /// symbol tables. This function returns None if there are any unknown
  /// operations that may potentially be symbol tables.
  static Optional<UseRange> getSymbolUses(Operation *from);
  static Optional<UseRange> getSymbolUses(Region *from);

  /// Get all of the uses of the given symbol that are nested within the given
  /// operation 'from'. This does not traverse into any nested symbol tables.
  /// This function returns None if there are any unknown operations that may
  /// potentially be symbol tables.
  static Optional<UseRange> getSymbolUses(StringRef symbol, Operation *from);
  static Optional<UseRange> getSymbolUses(Operation *symbol, Operation *from);
  static Optional<UseRange> getSymbolUses(StringRef symbol, Region *from);
  static Optional<UseRange> getSymbolUses(Operation *symbol, Region *from);

  /// Return if the given symbol is known to have no uses that are nested
  /// within the given operation 'from'. This does not traverse into any nested
  /// symbol tables. This function will also return false if there are any
  /// unknown operations that may potentially be symbol tables. This doesn't
  /// necessarily mean that there are no uses, we just can't conservatively
  /// prove it.
  static bool symbolKnownUseEmpty(StringRef symbol, Operation *from);
  static bool symbolKnownUseEmpty(Operation *symbol, Operation *from);
  static bool symbolKnownUseEmpty(StringRef symbol, Region *from);
  static bool symbolKnownUseEmpty(Operation *symbol, Region *from);

  /// Attempt to replace all uses of the given symbol 'oldSymbol' with the
  /// provided symbol 'newSymbol' that are nested within the given operation
  /// 'from'. This does not traverse into any nested symbol tables. If there are
  /// any unknown operations that may potentially be symbol tables, no uses are
  /// replaced and failure is returned.
  LLVM_NODISCARD static LogicalResult replaceAllSymbolUses(StringRef oldSymbol,
                                                           StringRef newSymbol,
                                                           Operation *from);
  LLVM_NODISCARD static LogicalResult
  replaceAllSymbolUses(Operation *oldSymbol, StringRef newSymbolName,
                       Operation *from);
  LLVM_NODISCARD static LogicalResult
  replaceAllSymbolUses(StringRef oldSymbol, StringRef newSymbol, Region *from);
  LLVM_NODISCARD static LogicalResult
  replaceAllSymbolUses(Operation *oldSymbol, StringRef newSymbolName,
                       Region *from);

private:
  Operation *symbolTableOp;

  /// This is a mapping from a name to the symbol with that name.
  llvm::StringMap<Operation *> symbolTable;

  /// This is used when name conflicts are detected.
  unsigned uniquingCounter = 0;
};

//===----------------------------------------------------------------------===//
// SymbolTable Trait Types
//===----------------------------------------------------------------------===//

namespace detail {
LogicalResult verifySymbolTable(Operation *op);
LogicalResult verifySymbol(Operation *op);
} // namespace detail

namespace OpTrait {
/// A trait used to provide symbol table functionalities to a region operation.
/// This operation must hold exactly 1 region. Once attached, all operations
/// that are directly within the region, i.e not including those within child
/// regions, that contain a 'SymbolTable::getSymbolAttrName()' StringAttr will
/// be verified to ensure that the names are uniqued. These operations must also
/// adhere to the constraints defined by the `Symbol` trait, even if they do not
/// inherit from it.
template <typename ConcreteType>
class SymbolTable : public TraitBase<ConcreteType, SymbolTable> {
public:
  static LogicalResult verifyTrait(Operation *op) {
    return ::mlir::detail::verifySymbolTable(op);
  }

  /// Look up a symbol with the specified name, returning null if no such
  /// name exists. Symbol names never include the @ on them. Note: This
  /// performs a linear scan of held symbols.
  Operation *lookupSymbol(StringRef name) {
    return mlir::SymbolTable::lookupSymbolIn(this->getOperation(), name);
  }
  template <typename T> T lookupSymbol(StringRef name) {
    return dyn_cast_or_null<T>(lookupSymbol(name));
  }
  Operation *lookupSymbol(SymbolRefAttr symbol) {
    return mlir::SymbolTable::lookupSymbolIn(this->getOperation(), symbol);
  }
  template <typename T>
  T lookupSymbol(SymbolRefAttr symbol) {
    return dyn_cast_or_null<T>(lookupSymbol(symbol));
  }
};

} // end namespace OpTrait
} // end namespace mlir

/// Include the generated symbol interfaces.
#include "mlir/IR/SymbolInterfaces.h.inc"

#endif // MLIR_IR_SYMBOLTABLE_H
