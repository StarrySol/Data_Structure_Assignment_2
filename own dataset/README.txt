STRESS TEST DATASETS FOR PROJECT 2 - APSC ALGORITHM
=====================================================
FORMAT: ring_id, vertex_id, x, y
  ring_id = 0  → exterior ring (counterclockwise)
  ring_id > 0  → holes (clockwise)

DATASET CATEGORIES:
-------------------

1. HIGH VERTEX COUNT (high_vertex_N.csv)
   N = 500, 1000, 5000, 10000
   → Tests heap scaling in SimplifyAll
   → Expected: O(n log n) for single rings
   → Property targeted: running time scaling

2. MANY HOLES (many_holes_N.csv)
   N = 5, 10, 20, 50 holes
   → IsCollapseValid loops over ALL rings per collapse
   → More holes = much slower intersection checks
   → Property targeted: O(n^2) worst-case behaviour

3. NEAR-DEGENERACIES (near_degenerate_N.csv)
   N = 200, 500, 1000
   → Nearly collinear vertices along top edge
   → Very low cost → heap pops many near-zero candidates
   → Property targeted: stability guard (moveDist > 500)

4. NARROW GAPS (narrow_gap_G.csv)
   G = 50, 10, 2, 0.5 (gap size)
   → Hole almost fills the exterior polygon
   → Many intersection checks will fail in IsCollapseValid
   → Property targeted: topology validity under tight constraints

5. FLAT ELLIPSE (flat_ellipse.csv)
   → Extremely flat shape (5000 wide, 10 tall)
   → AD nearly parallel to BC frequently
   → Property targeted: ComputeNewPoint degenerate case

6. COMBINED STRESS (combined_stress.csv)
   → 15 holes + 200-vertex noisy exterior
   → Tests everything at once
   → Property targeted: real-world complex polygon
