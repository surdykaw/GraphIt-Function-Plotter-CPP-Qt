# Architecture & Plot Generation Pipeline
## 1. Conceptual model

The application represents a **continuous mathematical coordinate plane** rendered inside a movable viewport.   
The screen does not represent the coordinate system itself.
Instead:

> User moves the viewport over a large finite 2D plane.

All visual elements (grid, axes, labels, plots) exist in the **coordinate plane** and are projected into screen coordinates during rendering.

---
## 2. Core components

The program is divided into three main layers:

```
MainWin  →  GraphLayer  →  Function
(UI)        (Rendering)    (Math model)
```

---
### MainWin — Application Controller (UI layer)

Responsible for:

* managing user input
* maintaining the list of plotted functions
* validating and parsing parameters
* scheduling recalculation (debounced updates)

This class never draws graphs and never evaluates math expressions.

It only decides:

> which functions should exist

When any of the parameters change:

1. Existing Function object is destroyed
2. New Function object is created
3. GraphLayer updates automatically

So the UI owns **function lifetime**, not rendering.

---
### GraphLayer — Coordinate Plane Renderer (View layer)

Represents the visible window into the mathematical plane.
Responsibilities:

* rendering infinite grid
* rendering axes
* mapping mathematical coordinates to screen coordinates
* handling viewport dragging (panning)
* drawing all functions

GraphLayer does NOT compute function values.
It only draws data prepared by Function objects.

Important property:

> GraphLayer is stateless regarding math — it only visualizes data.

---
### Function — Mathematical Model (Data layer)

Encapsulates a single mathematical function.

Responsibilities:

* parsing expression (ExprTk - see external/exprtk.h)
* sampling domain
* calculating function values
* converting results into a drawable polyline

After construction the function becomes immutable.

The object stores:

```
-> domain values
-> calculated values
-> precomputed geometry
```

GraphLayer only renders the stored polyline — no runtime evaluation occurs during painting.

---
## 3. Rendering pipeline
### Step 1 — User input
User edits formula or domain range.

### Step 2 — Debounced rebuild
After a short delay:

```
MainWin → destroys old Function
MainWin → creates new Function
```

### Step 3 — Precomputation
Function:

```
-> parse expression
-> sample domain
-> compute values
-> build Qt polyline
```

### Step 4 — Rendering
At every frame:

```
GraphLayer.paintEvent()
    -> draw grid
    -> draw axes
    -> draw labels
    -> draw precomputed polylines
```

No mathematical calculations happen during painting.

---
## 4. Coordinate system

The mathematical plane uses a centered Cartesian coordinate system:

```
(0,0) located at the middle of the GraphLayer
INTERSPACE = pixels per one integer unit
```

Conversion:

```
screenX = originX + coordinateX * INTERSPACE
screenY = originY - coordinateY * INTERSPACE
```

The viewport moves by shifting the GraphLayer widget.

---
## 5. Performance design decisions
### Precomputation model

Function values are calculated once during creation.
Painting only renders stored geometry.
Result:

* deterministic frame time
* no UI stuttering while dragging
* complexity independent of expression complexity

---
### Sampling strategy

The function is sampled at fixed mathematical intervals based on pixel density:

```
ARG_STEP_VALUE = 2.0 / static_cast<double>(INTERSPACE);
```

which guarantees consistent visual density.

---
### Memory ownership

Ownership rules:

| Object     | Owns                      |
| ---------- | ------------------------- |
| MainWin    | Function instances        |
| GraphLayer | nothing (only references) |
| Function   | its geometry data         |

Destruction of a UI row destroys exactly one Function.

---
## 6. Interaction model

Dragging does not move functions.
Dragging moves the viewport. Therefore:

* the mathematical plane stays static
* only projection changes

This avoids recalculation during navigation.

---
## 7. Threading model

Single-threaded UI application.

Performance is achieved through:

* precomputation
* immutable geometry
* constant-time rendering

---
## Architectural summary

The program follows a simplified MVC-like separation:

| Layer      | Role       |
| ---------- | ---------- |
| MainWin    | Controller |
| GraphLayer | View       |
| Function   | Model      |

The renderer never evaluates math, and the math layer never interacts with UI.  
This separation guarantees predictable rendering performance and simplifies debugging.