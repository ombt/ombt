#  Semaphore.tcl
#  Impementation of Semaphores
#
#  ::Semaphore::Signal token
#  ::Semaphore::Wait   ?pattern? ?pattern? ...   - returns token
#  ::Semaphore::Reset  token

package provide Semaphore 2.3

# Semaphore tokens may only contain the same characters as TCL variables.
# Specifically, tokens excluding interpreter paths may not contain the / character.
#
# 2.3 - March 24, 2003
#     - added the ability to use this package across interpreters
#       Semaphore tokens of child interpreters are of the form
#       child/child/token where {child/child} is the interpreter path.
#       ../../token can be used to refer to parent Semaphores
#
# 2.2 - Mar 6, 2003
#     - added the ability to wait on a number of patterns
#
# 2.1 - October 22, 2000
#     - fixed a bug that would result in a very slow memory leak

namespace eval ::Semaphore {
  variable SemData
  array set SemData {}
  variable SemAny 0

  namespace export Signal Wait Reset 
  namespace export Extend 
}

# =========================================================
# Internal routines to peform the Semaphore control
# --------------------------------------------------------- _Signal
# Signal a Semaphore
# _Signal { SemToken }
proc ::Semaphore::_Signal { SemToken } {
  variable SemData
  variable SemAny

  if {![info exist SemData($SemToken)]} then {
     set retValue [set SemData($SemToken) 1]
  } else {
     set retValue [incr SemData($SemToken)]
  }
  incr SemAny
  return $retValue
}
  
# --------------------------------------------------------- _Wait
# Wait for a Semaphore
# Note: Resetting a Semaphore does not signal it.
#
# _Wait { TokenList }
proc ::Semaphore::_Wait { TokenList } {
  variable SemData
  variable SemAny

  if {[llength $TokenList] == 0} then {
    set Patterns *
  } else {
    set Patterns $TokenList
  }
  set FoundFlag 0
  set FoundName ""
  if {$SemAny != 0} then {
    foreach Pattern $Patterns {
      foreach n [array names SemData $Pattern] {
        if {$SemData($n) > 0} then {
          set FoundName $n
          set FoundFlag 1
          break
        }
      }
      if {$FoundFlag == 1} then {
        break
      }
    }
  }
  while {!$FoundFlag} {
    vwait ::Semaphore::SemAny
    foreach Pattern $Patterns {
      foreach n [array names SemData $Pattern] {
        if {$SemData($n) > 0} then {
          set FoundName $n
          set FoundFlag 1
          break
        }
      }
      if {$FoundFlag == 1} then {
        break
      }
    }
  }

  if {[incr SemData($FoundName) -1] == 0} then {
    unset SemData($FoundName)
  }
  incr SemAny -1
  return $FoundName
}
  
# --------------------------------------------------------- _Reset
# Reset a Semaphore
# Note: Resetting a Semaphore does not signal it.
#
# _Reset { SemToken }
proc ::Semaphore::_Reset { SemToken } {
  variable SemData
  variable SemAny

  if {[info exist SemData($SemToken)]} then {
    incr SemAny -$SemData($SemToken)
    incr SemData($SemToken) -$SemData($SemToken)
  }
}

# ==================================================================
# these routines are used from slave interpreters to collect
# the interpreter names and then call the internal _signal/_wait
# routines. These may in turn be aliased to a parent.

# --------------------------------------------------------- _SlaveSignal
# _SlaveSignal interp SemToken
proc ::Semaphore::_SlaveSignal { interp SemToken } {

  # if the Semaphore is of the form ../* then remove one level
  # of interp call and the ../ from the token
  if {[regexp {\.\./(.*)} $SemToken dummy remaining]} then {
    return [::Semaphore::_Signal $remaining]
  } else {
    return [::Semaphore::_Signal $interp/$SemToken]
  }
}

# --------------------------------------------------------- _SlaveWait
# _SlaveWait interp SemToken ...
proc ::Semaphore::_SlaveWait { interp SemTokenList } {

  set SemTokens ""
  foreach token $SemTokenList {
    # if the Semaphore is of the for ../* then remove one level
    # of interp call and the ../ from the token
    if {[regexp {\.\./(.*)} $token dummy remaining]} then {
      lappend SemTokens $remaining
    } else {
      lappend SemTokens $interp/$token
    }
  }
  set returnSemtoken [::Semaphore::_Wait $SemTokens]
  # if there is no interp in the returned token, add the ../ parent indicator.
  if {([string first "/" $returnSemtoken] == -1) || ([string match {../*} $returnSemtoken])} then {
    # get rid of the leading interp name
    return ../$returnSemtoken
  } else {
    return [string range $returnSemtoken [expr [string first "/" $returnSemtoken] +1] end]
  }
}

# --------------------------------------------------------- _SlaveReset
# _SlaveReset interp interp ... SemToken
proc ::Semaphore::_SlaveReset { interp SemToken } {

  # if the Semaphore is of the form ../* then remove one level
  # of interp call and the ../ from the token
  if {[regexp {\.\./(.*)} $SemToken dummy remaining]} then {
    return ../[::Semaphore::_Reset $remaining]
  } else {
    set returnSemtoken [::Semaphore::_Reset $interp/$SemToken]
    # get rid of the leading interp name
    return [string range $returnSemtoken [expr [string first "/" $returnSemtoken] +1] end]
  }
}


# ==================================================================
# These routines remap the local signal/wait routines
# into the _signal/_wait routines which are interpreter
# aware.
#
# ------------------------------------------------------ Signal ----
#  Signal {interp path} {SemToken}
proc ::Semaphore::Signal  { SemToken } {
  return [::Semaphore::_Signal $SemToken]
}

# ------------------------------------------------------ Reset ----
proc ::Semaphore::Reset  { SemToken } {
  return [::Semaphore::_Reset $SemToken]
}

# ------------------------------------------------------ Wait ----
proc ::Semaphore::Wait  { args } {
  return [::Semaphore::_Wait $args]
}

# ==================================================================
# extend the current non-local Semaphores to a child interpreter

# ------------------------------------------------------ Extend ----
proc ::Semaphore::Extend { Interp } {

  if {[llength $Interp] == 1} {
    interp eval $Interp { package require Semaphore }

    # alias the local Semaphore aware routine to the parent routines
    # that add and remove the child interp names from Semaphore names
    $Interp alias ::Semaphore::_Signal ::Semaphore::_SlaveSignal $Interp
    $Interp alias ::Semaphore::_Wait ::Semaphore::_SlaveWait $Interp
    $Interp alias ::Semaphore::_Reset ::Semaphore::_SlaveReset $Interp

  } else {
    error "Can't extend Semaphores to other than immediate slave interpreters."
  }
}