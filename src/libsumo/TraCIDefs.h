/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2012-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    TraCIDefs.h
/// @author  Daniel Krajzewicz
/// @author  Mario Krumnow
/// @author  Michael Behrisch
/// @author  Robert Hilbrich
/// @date    30.05.2012
/// @version $Id$
///
// C++ TraCI client API implementation
/****************************************************************************/
#ifndef TraCIDefs_h
#define TraCIDefs_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <vector>
#include <limits>
#include <map>
#include <string>
#include <stdexcept>
#include <sstream>
#include <memory>


// ===========================================================================
// global definitions
// ===========================================================================
#define DEFAULT_VIEW "View #0"
#define PRECISION 2


// ===========================================================================
// class and type definitions
// ===========================================================================
typedef long long int SUMOTime; // <utils/common/SUMOTime.h>
#define SUMOTime_MAX std::numeric_limits<SUMOTime>::max()

namespace libsumo {
/**
* @class TraCIException
*/
class TraCIException : public std::runtime_error {
public:
    /** constructor */
    TraCIException(std::string what)
        : std::runtime_error(what) {}
};

/// @name Structures definitions
/// @{

struct TraCIResult {
    virtual std::string getString() {
        return "";
    }
};

/** @struct TraCIPosition
    * @brief A 3D-position
    */
struct TraCIPosition : TraCIResult {
    std::string getString() {
        std::ostringstream os;
        os << "TraCIPosition(" << x << "," << y << "," << z << ")";
        return os.str();
    }
    double x, y, z;
};

/** @struct TraCIRoadPosition
    * @brief An edgeId, position and laneIndex
    */
struct TraCIRoadPosition : TraCIResult {
    std::string getString() {
        std::ostringstream os;
        os << "TraCIRoadPosition(" << edgeID << "_" << laneIndex << "," << pos << ")";
        return os.str();
    }
    std::string edgeID;
    double pos;
    int laneIndex;
};

/** @struct TraCIColor
    * @brief A color
    */
struct TraCIColor : TraCIResult {
    std::string getString() {
        std::ostringstream os;
        os << "TraCIColor(" << r << "," << g << "," << b << "," << a << ")";
        return os.str();
    }
    unsigned char r, g, b, a;
};

/** @struct TraCIPositionVector
    * @brief A list of positions
    */
typedef std::vector<TraCIPosition> TraCIPositionVector;

/** @struct TraCIBoundary
    * @brief A 3D-bounding box
    */
struct TraCIBoundary {
    double xMin, yMin, zMin;
    double xMax, yMax, zMax;
};


struct TraCIInt : TraCIResult {
    TraCIInt() : value(0) {}
    TraCIInt(int v) : value(v) {}
    std::string getString() {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    int value;
};


struct TraCIDouble : TraCIResult {
    TraCIDouble() : value(0.) {}
    TraCIDouble(double v) : value(v) {}
    std::string getString() {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    double value;
};


struct TraCIString : TraCIResult {
    TraCIString() : value("") {}
    TraCIString(std::string v) : value(v) {}
    std::string getString() {
        return value;
    }
    std::string value;
};


struct TraCIStringList : TraCIResult {
    std::string getString() {
        std::ostringstream os;
        os << "[";
        for (std::string v : value) {
            os << v << ",";
        }
        os << "]";
        return os.str();
    }
    std::vector<std::string> value;
};


/// @brief {variable->value}
typedef std::map<int, std::shared_ptr<TraCIResult> > TraCIResults;
/// @brief {object->{variable->value}}
typedef std::map<std::string, TraCIResults> SubscriptionResults;
typedef std::map<std::string, SubscriptionResults> ContextSubscriptionResults;


class TraCIPhase {
public:
    TraCIPhase(const SUMOTime _duration, const SUMOTime _duration1, const SUMOTime _duration2, const std::string& _phase)
        : duration(_duration), duration1(_duration1), duration2(_duration2), phase(_phase) {}
    ~TraCIPhase() {}

    SUMOTime duration, duration1, duration2;
    std::string phase;
};


class TraCILogic {
public:
    TraCILogic() {}
    TraCILogic(const std::string& _subID, int _type, int _currentPhaseIndex)
        : subID(_subID), type(_type), currentPhaseIndex(_currentPhaseIndex) {}
    ~TraCILogic() {}

    std::string subID;
    int type;
    int currentPhaseIndex;
    std::vector<TraCIPhase> phases;
    std::map<std::string, std::string> subParameter;
};


class TraCILink {
public:
    TraCILink(const std::string& _from, const std::string& _via, const std::string& _to)
        : fromLane(_from), viaLane(_via), toLane(_to) {}
    ~TraCILink() {}

    std::string fromLane;
    std::string viaLane;
    std::string toLane;
};


class TraCIConnection {
public:
    TraCIConnection(const std::string& _approachedLane, const bool _hasPrio, const bool _isOpen, const bool _hasFoe,
                    const std::string _approachedInternal, const std::string _state, const std::string _direction, const double _length)
        : approachedLane(_approachedLane), hasPrio(_hasPrio), isOpen(_isOpen), hasFoe(_hasFoe),
          approachedInternal(_approachedInternal), state(_state), direction(_direction), length(_length) {}
    ~TraCIConnection() {}

    std::string approachedLane;
    bool hasPrio;
    bool isOpen;
    bool hasFoe;
    std::string approachedInternal;
    std::string state;
    std::string direction;
    double length;
};


/// @brief mirrors MSInductLoop::VehicleData
struct TraCIVehicleData {
    /// @brief The id of the vehicle
    std::string id;
    /// @brief Length of the vehicle
    double length;
    /// @brief Entry-time of the vehicle in [s]
    double entryTime;
    /// @brief Leave-time of the vehicle in [s]
    double leaveTime;
    /// @brief Type of the vehicle in
    std::string typeID;
};


struct TraCINextTLSData {
    /// @brief The id of the next tls
    std::string id;
    /// @brief The tls index of the controlled link
    int tlIndex;
    /// @brief The distance to the tls
    double dist;
    /// @brief The current state of the tls
    char state;
};


struct TraCIBestLanesData {
    /// @brief The id of the lane
    std::string laneID;
    /// @brief The length than can be driven from that lane without lane change
    double length;
    /// @brief The traffic density along length
    double occupation;
    /// @brief The offset of this lane from the best lane
    int bestLaneOffset;
    /// @brief Whether this lane allows continuing the route
    bool allowsContinuation;
    /// @brief The sequence of lanes that best allows continuing the route without lane change
    std::vector<std::string> continuationLanes;
};


class TraCIStage {
public:
    TraCIStage() {} // only to make swig happy
    TraCIStage(int _type) : type(_type), depart(-1) {}
    /// @brief The type of stage (walking, driving, ...)
    int type;
    /// @brief The line or the id of the vehicle type
    std::string line;
    /// @brief The id of the destination stop
    std::string destStop;
    /// @brief The sequence of edges to travel
    std::vector<std::string> edges;
    /// @brief duration of the stage
    double travelTime;
    /// @brief effort needed
    double cost;
    /// @brief id of the intended vehicle for public transport ride
    std::string intended;
    /// @brief intended depart time for public transport ride or -1
    double depart;
};
}


#endif

/****************************************************************************/
