 ImGui Tooling

  - Repurpose the stub windows in src/gui/imguihandler.cpp:47-57:
      - Control Panel: run/pause, mutation rate sliders, director mode toggles, encounter presets.
      - "Crypto Chart" window → Encounter Metrics View: plot boss HP, threat distribution, and reward curves per tick (ImPlot works well once added).
      - Network Viewer: switchable tabs for healer/tank/DPS genomes, with overlays for currently firing neurons and inputs coming from the director.

  Recommended Next Steps

  1. Flesh out src/neat_core with genome/species/population scaffolding plus serialization so you can persist runs.
  2. Stub simulation entities and a basic deterministic boss so you can validate the control loop before adding NEAT.
  3. Implement a simple rule-based swarm director to define fitness shaping; evolve NEAT agents against it.
  4. Hook encounter/fitness diagnostics into ImGui for fast iteration.
  5. Once the loop is stable, iterate on sensory inputs, ability sets, and boss mechanics to push emergent teamwork.
