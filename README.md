# ScriptBots

**Author:** Andrej Karpathy  
**Contributors:** Casey Link  
**License:** BSD

A sophisticated artificial life simulation featuring evolving neural network agents in a 2D world with complex behaviors, food systems, and evolutionary dynamics.

## Overview

ScriptBots is an artificial life simulation that demonstrates emergent behaviors through the evolution of neural network-controlled agents. The simulation features:

- **Neural Network Brains**: Each agent has a Damped Weighted Recurrent AND/OR Network (MLPBrain) that controls their behavior
- **Evolutionary Dynamics**: Agents reproduce, mutate, and evolve over generations
- **Complex Ecosystems**: Herbivores and carnivores with different survival strategies
- **Food Systems**: Dynamic food distribution, sharing, and consumption mechanics
- **Real-time Visualization**: OpenGL-based rendering with interactive controls
- **Save/Load System**: Persistent simulation states with lineage tracking

## Architecture

### Core Components

#### World (`World.h/cpp`)
The main simulation container that manages:
- Agent population and lifecycle
- Food distribution and dynamics
- Collision detection and physics
- Reproduction and evolution mechanics
- Save/load functionality

#### Agent (`Agent.h/cpp`)
Individual entities with:
- **Physical Properties**: Position, health, age, visual characteristics
- **Neural Brain**: MLPBrain for decision making
- **Sensory System**: Vision, sound, smell, proximity detection
- **Behavioral Outputs**: Movement, attack spikes, food sharing
- **Evolutionary Traits**: Mutation rates, lineage tracking, genetic inheritance

#### MLPBrain (`MLPBrain.h/cpp`)
A sophisticated neural network implementation:
- **Recurrent Architecture**: Damped weighted connections with memory
- **Dynamic Synapses**: Change-sensitive connections that adapt over time
- **Evolutionary Learning**: Mutation and crossover operations
- **Flexible Topology**: Variable network size and connectivity

#### Configuration System (`Config.h/cpp`)
Comprehensive parameter management:
- **Runtime Configuration**: No recompilation needed for parameter changes
- **Cached Performance**: Optimized access to frequently used values
- **Save Compatibility**: Configuration state preservation
- **Extensible Design**: Easy addition of new parameters

### Key Features

#### Neural Network Design
- **Input Neurons**: Vision (RGB + proximity for each eye), sound, smell, health
- **Output Neurons**: Left/right wheel control, RGB color, spike activation
- **Hidden Layers**: Evolvable topology with recurrent connections
- **Learning**: Continuous adaptation through mutation and crossover

#### Evolutionary System
- **Reproduction**: Health-based reproduction with configurable rates
- **Mutation**: Multi-level mutation affecting brain structure and behavior
- **Crossover**: Genetic recombination between successful agents
- **Lineage Tracking**: Unique tags for tracking evolutionary history

#### Food Dynamics
- **Growth**: Food tiles regenerate over time
- **Sharing**: Agents can transfer food to nearby individuals
- **Distribution**: Dead agents' bodies become food for others
- **Competition**: Limited resources drive evolutionary pressure

## Building

### Prerequisites

- **C++11** compatible compiler (GCC 4.8+ or Clang 3.3+)
- **OpenGL** and **GLUT** libraries
- **OpenMP** for parallel processing (optional but recommended)

### Dependencies

### Scriptbots is currently developed for use on Linux

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

### Compilation

1. **Clone the repository**:
   ```
   git clone <repository-url>
   cd scriptbots
   ```

2. **Build the project**:
   ```
   make
   ```

3. **Run the simulation**:
   ```
   ./scriptbots
   ```

### Build Options

- **Clean build**: `make clean`
- **Install dependencies**: `make install-deps`
- **Generate version**: `make generate-version`

## Usage

### Basic Controls

- **Mouse**: Pan and zoom around the world
- **'d'**: Toggle fast mode (disables drawing for much faster simulation)
- **'+'/'-'**: Adjust simulation speed
- **'r'**: Reset the simulation
- **'s'**: Save current state
- **'l'**: Load saved state

### Configuration

The simulation is highly configurable through `scriptbots.conf`. Key parameters include:

#### World Settings
- `world.width/height`: Simulation world dimensions
- `world.window_width/height`: Display window size
- `world.cell_size`: Food tile size

#### Agent Behavior
- `agent.speed`: Movement speed
- `agent.spike_speed`: Attack spike extension rate
- `agent.herbivore_reproduction_rate`: Reproduction frequency
- `agent.carnivore_reproduction_rate`: Carnivore reproduction rate

#### Evolution
- `evolution.meta_mutation_rate_1/2`: Mutation rate evolution
- `evolution.reproduction_multiplier`: Reproduction incentive

#### Food System
- `food.intake_rate`: Food consumption rate
- `food.sharing_distance`: Food sharing range
- `food.add_frequency`: Food regeneration rate

### Command Line Options

- `--help`: Display help information
- `--version`: Show version information
- `--load <file>`: Load simulation from file
- `--config <file>`: Use custom configuration file

## Simulation Mechanics

### Agent Lifecycle

1. **Birth**: Agents spawn with random or inherited neural networks
2. **Sensing**: Agents perceive their environment through multiple sensors
3. **Decision**: Neural networks process inputs to determine actions
4. **Action**: Agents move, attack, share food, or reproduce
5. **Evolution**: Successful agents pass their genes to offspring
6. **Death**: Agents die from starvation, predation, or old age

### Neural Network Operation

1. **Input Processing**: Sensory data normalized and fed to input neurons
2. **Network Computation**: Recurrent connections process information
3. **Output Generation**: Motor neurons control agent behavior
4. **Learning**: Network weights adapt through evolutionary pressure

### Evolutionary Dynamics

- **Selection Pressure**: Limited resources and predation drive evolution
- **Genetic Diversity**: Mutation and crossover maintain population variety
- **Emergent Behaviors**: Complex strategies emerge from simple rules
- **Coevolution**: Herbivores and carnivores evolve in response to each other

## Development

### Adding New Features

1. **Configuration**: Add new parameters to `Config.h` and `scriptbots.conf`
2. **Agent Behavior**: Extend `Agent.h/cpp` with new capabilities
3. **Neural Networks**: Modify `MLPBrain.h/cpp` for new brain features
4. **Visualization**: Update `GLView.h/cpp` for new rendering features

### Code Style

- **C++11**: Use modern C++ features where appropriate
- **Naming**: Clear, descriptive names for functions and variables
- **Documentation**: Comment complex algorithms and design decisions
- **Performance**: Use OpenMP for parallel processing where beneficial

## Troubleshooting

### Common Issues

1. **OpenGL Errors**: Ensure proper graphics drivers and OpenGL libraries
2. **Compilation Errors**: Check C++11 compatibility and dependency installation
3. **Performance Issues**: Enable fast mode with 'd' key for better performance
4. **Configuration Issues**: Verify `scriptbots.conf` syntax and parameter values

### Performance Optimization

- **Fast Mode**: Disable rendering for maximum simulation speed
- **OpenMP**: Multi-core processing for agent updates
- **Configuration**: Adjust world size and agent count for your system

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes with appropriate documentation
4. Test thoroughly with different configurations
5. Submit a pull request with clear description

## License

This project is licensed under the BSD License - see the LICENSE file for details.

## Acknowledgments

- **Andrej Karpathy**: Original design and implementation
- **Casey Link**: Contributions and improvements
- **OpenGL Community**: Graphics rendering capabilities
- **Artificial Life Community**: Inspiration and research foundation

---

For questions, comments, or contributions, please refer to the project repository or contact the maintainers. 